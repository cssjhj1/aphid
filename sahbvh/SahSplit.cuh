#include <cuda_runtime_api.h>
#include "SimpleQueue.cuh"
#include "sah_math.cuh"
#include "onebitsort.cuh"

namespace sahsplit {

struct DataInterface {
    int2 * nodes;
    Aabb * nodeAabbs;
    int * nodeParents;
    int * nodeLevels;
    KeyValuePair * primitiveIndirections;
    Aabb * primitiveAabbs;
    KeyValuePair * intermediateIndirections;
};

struct SplitTask {
template <typename QueueType, int NumBins, int NumThreads>
    __device__ int execute(QueueType * q,
                            DataInterface data,
                            int * smem)
    {
        int & sWorkPerBlock = smem[0];
        
        computeBestBin<NumBins, NumThreads>(sWorkPerBlock, data, smem);
        
        if(validateSplit(&smem[1]))
            rearrange<NumBins, NumThreads>(data, smem);

        if(threadIdx.x == 0) {  
            if(validateSplit(&smem[1]))
                spawn(q, data, smem);
                
            q->setWorkDone();
            q->swapTails();

        }
       // __threadfence();
       // __syncthreads();
        
        return 1;   
    }
   
template<int NumBins, int NumThreads>
    __device__ void computeBestBin(int iRoot,
                                    DataInterface data,
                                    int * smem)
    {
        int2 root = data.nodes[iRoot];
        Aabb rootBox = data.nodeAabbs[iRoot];
/*
 *    layout of memory in int
 *    n  as num bins
 *    t  as num threads
 *    16 as size of bin
 *
 *    0                                      workId
 *    1             -> 1+3*16-1              best bin per dimension
 *    1+3*16        -> 1+3*16+3-1            cost of best bin per dimension
 *
 */             
        computeBestBinPerDimension<NumBins, NumThreads, 0>(data,
                                    smem,
                                    root,
                                    rootBox);
        
        computeBestBinPerDimension<NumBins, NumThreads, 1>(data,
                                    smem,
                                    root,
                                    rootBox);
        
        computeBestBinPerDimension<NumBins, NumThreads, 2>(data,
                                    smem,
                                    root,
                                    rootBox);
        
        SplitBin * sBestBin = (SplitBin *)&smem[1];
        float * sBestCost = (float *)&smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT];
        
        if(threadIdx.x < 1) {
// first is the best
            if(sBestCost[1] < sBestCost[0])
                sBestBin[0] = sBestBin[1];
            
            if(sBestCost[2] < sBestCost[0])
                sBestBin[0] = sBestBin[2];
        }
    }
    
template<int NumBins, int NumThreads, int Dimension>
    __device__ void computeBestBinPerDimension(DataInterface data,
                                    int * smem,
                                    int2 root,
                                    Aabb rootBox)
    {
        if((root.y - root.x) < 16)
            computeBinsPerDimensionPrimitiveCenter<Dimension>(root.y - root.x+1,
                                    data,
                                    smem,
                                    root,
                                    rootBox);
        else
            computeBinsPerDimensionBatched<NumBins, NumThreads, Dimension>(data,
                                    smem,
                                    root,
                                    rootBox);
    }
 
template<int Dimension>  
    __device__ void computeBinsPerDimensionPrimitiveCenter(int NumBins,
                                    DataInterface data,
                                    int * smem,
                                    int2 root,
                                    Aabb rootBox)
    {
/*
 *    layout of memory in int
 *    n  as num bins and num primitives
 *    16 as size of bin
 *
 *    0                                          workId
 *    1               -> 1+3*16-1                best bin per dimension
 *    1+3*16          -> 1+3*16+3-1              cost of best bin per dimension
 *    1+3*16+3        -> 1+3*16+3+n*16-1         bins
 *    1+3*16+3+n*16   -> 1+3*16+3+n*16+n-1       costs
 *    1+3*16+3+n*16+n -> 1+3*16+3+n*16+n+n*n-1   sides
 *
 *    when n = 16
 *    total shared memory 2320 bytes
 *
 */ 
        SplitBin * sBestBin = (SplitBin *)&smem[1];
        float * sBestCost = (float *)&smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT];
        SplitBin * sBin = (SplitBin *)&smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT + 3];
        float * sCost = (float *)&smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT + 3
                                        + NumBins * SIZE_OF_SPLITBIN_IN_INT];
        int * sSide = &smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT + 3
                                        + NumBins * SIZE_OF_SPLITBIN_IN_INT
                                        + NumBins];
/*
 *    layout of sides
 *    0    n     2n    3n
 *    1    n+1   2n+1  3n+1
 *   
 *    n-1  2n-1  3n-1  4n-1
 *
 *    vertical computeSides
 *    horizonal collectBins
 */
        int * sideVertical = &sSide[NumBins * threadIdx.x];
        int * sideHorizontal = &sSide[threadIdx.x];                      
        
        KeyValuePair * primitiveIndirections = data.primitiveIndirections;
        Aabb * primitiveAabbs = data.primitiveAabbs;
        Aabb clusterBox;
        float3 center;
        if(threadIdx.x < NumBins) {
            resetSplitBin(sBin[threadIdx.x]);
// primitive center as split plane             
            clusterBox = primitiveAabbs[primitiveIndirections[root.x + threadIdx.x].value];
            center = centroidOfAabb(clusterBox);
            sBin[threadIdx.x].plane = float3_component(center, Dimension);
        }
        
        __syncthreads();
        
/*
 *   n as num bins and num primitives
 *
 *   0   1     2     ..     n-1
 *
 *   0   n     2n          (n-1)n
 *   1   n+1   2n+1        (n-1)n+1
 *   2   n+2   2n+2        (n-1)n+2
 *   .
 *   .
 *   n-1 2n-1  3n-1           nn-1
 *
 *   horizontal i as primitives
 *   vertical   j as bins
 */             
        int i, j;
        if(threadIdx.x < NumBins*NumBins) {
            i = threadIdx.x / NumBins;
            j = threadIdx.x - i * NumBins;
            
            clusterBox = primitiveAabbs[primitiveIndirections[root.x + i].value];
            center = centroidOfAabb(clusterBox);
            
            sideVertical[j] = (float3_component(center, Dimension) > sBin[j].plane);
        }
    
        __syncthreads();
    
        if(threadIdx.x < NumBins) {
            for(i=0; i<NumBins; i++) {
                clusterBox = primitiveAabbs[primitiveIndirections[root.x + i].value];
                updateSplitBinSide(sBin[threadIdx.x], clusterBox, 
                                    sideHorizontal[i * NumBins]);
            }
        }

        __syncthreads();
        
        
        if(threadIdx.x < NumBins) {
            float rootArea = areaOfAabb(&rootBox);
            sCost[threadIdx.x] = costOfSplit(&sBin[threadIdx.x],
                                        rootArea);
        }
        
        __syncthreads();
           
        if(threadIdx.x < 1) {
            int bestI = 0;
            float lowestCost = sCost[0];
            for(i=1; i< NumBins; i++) {
                if(lowestCost > sCost[i]) {
                    lowestCost = sCost[i];
                    bestI = i;
                }
            }
            sBestBin[Dimension] = sBin[bestI];
            sBestBin[Dimension].dimension = Dimension;
// store cost here
            sBestCost[Dimension] = sCost[bestI];
        }
        
        __syncthreads();
    }
    
template<int NumBins, int NumThreads, int Dimension>  
    __device__ void computeBinsPerDimensionBatched(DataInterface data,
                                    int * smem,
                                    int2 root,
                                    Aabb rootBox)
    {
/*
 *    layout of memory in int
 *    n  as num bins
 *    t  as num threads
 *    16 as size of bin
 *    32 as warp size
 *
 *    0                                          workId
 *    1                   -> 1+3*16-1                best bin per dimension
 *    1+3*16              -> 1+3*16+3-1                 cost of best bin per dimension
 *    1+3*16+3            -> 1+3*16+3+n*(t/32)*16-1               bins
 *    1+3*16+3+n*(t/32)*16       -> 1+3*16+3+n*(t/32)*16+n-1               costs
 *    1+3*16+3+n*(t/32)*16+n     -> 1+3*16+3+n*(t/32)*16+n+n*t-1             sides
 *
 *    when n = 8, t = 256
 *    total shared memory 12528 bytes
 */         
        SplitBin * sBestBin = (SplitBin *)&smem[1];
        float * sBestCost = (float *)&smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT];
        SplitBin * sBin = (SplitBin *)&smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT + 3];
        const int numWarps = NumThreads>>5;
        float * sCost = (float *)&smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT + 3
                                        + NumBins * numWarps * SIZE_OF_SPLITBIN_IN_INT];
        int * sSide = &smem[1 + 3 * SIZE_OF_SPLITBIN_IN_INT + 3
                                        + NumBins * numWarps * SIZE_OF_SPLITBIN_IN_INT
                                        + NumBins];
/*
 *    layout of sides
 *    0    n     2n    3n
 *    1    n+1   2n+1  3n+1
 *   
 *    n-1  2n-1  3n-1  4n-1
 *
 *    vertical computeSides
 *    horizonal collectBins
 *
 */
        int * sideVertical = &sSide[NumBins * threadIdx.x];
        int * sideHorizontal = &sSide[threadIdx.x];
        
        const int numWarpBins = numWarps * NumBins;
        
        if(threadIdx.x < numWarpBins)
            resetSplitBin(sBin[threadIdx.x]);
        
        if(threadIdx.x < NumBins)
            sBin[threadIdx.x].plane = binSplitPlane<Dimension>(&rootBox,
                                         NumBins,
                                         threadIdx.x);
        
        __syncthreads();
        
        KeyValuePair * primitiveIndirections = data.primitiveIndirections;
        Aabb * primitiveAabbs = data.primitiveAabbs;
                                    
        int nbatch = numBatches<NumThreads>(root);

        int i,j;
        for(i=0;i<nbatch;i++) {
            computeSides<NumBins, Dimension>(sideVertical,
                       rootBox,
                       primitiveIndirections,
                       primitiveAabbs,
                       root.x + i * NumThreads,
                       root.y);
        
            __syncthreads();
/*
 *    layout of warp bins
 *    n as num bins
 *    w as num warps (256/32)
 *
 *    0     1        2          w-1         w  warps
 *
 *    0     32n      64n       (w-1)32n
 *    1     32n+1    64n+1     (w-1)32n+1
 *    
 *    n-1   64n-1    96n-1      w32n
 *
 */
            /*
            if(threadIdx.x < numWarpBins) {
                  int w = threadIdx.x / NumBins;
                  int b = threadIdx.x - w * NumBins;
                  
                collectBinsInWarp<NumBins>(sBin[threadIdx.x], 
                    primitiveIndirections,
                    primitiveAabbs,
                    &sSide[b + w * 32 * NumBins],
                    root.x + w * 32 + i * NumThreads,
                    root.y);
            }
            
            __syncthreads();
            
            if(threadIdx.x < NumBins) {
                for(j=1; j<numWarps; j++)
                    combineSplitBin(sBin[threadIdx.x],
                                    sBin[threadIdx.x + j * NumBins]);
                  
            }
            
            __syncthreads();
            */
        
            if(threadIdx.x < NumBins) {   
                collectBins<NumBins, NumThreads>(sBin[threadIdx.x],
                    primitiveIndirections,
                    primitiveAabbs,
                    sideHorizontal,
                    root.x + i * NumThreads,
                    root.y);
            }
    
            __syncthreads();
        }
        
        if(threadIdx.x < NumBins) {
            float rootArea = areaOfAabb(&rootBox);
            sCost[threadIdx.x] = costOfSplit(&sBin[threadIdx.x],
                                        rootArea);
        }
        
         __syncthreads();
            
        if(threadIdx.x < 1) {
            int bestI = 0;
            float lowestCost = sCost[0];
            for(i=1; i< NumBins; i++) {
                if(lowestCost > sCost[i]) {
                    lowestCost = sCost[i];
                    bestI = i;
                }
            }
            sBestBin[Dimension] = sBin[bestI];
            sBestBin[Dimension].dimension = Dimension;
// store cost here
            sBestCost[Dimension] = sCost[bestI];
        }
        
        __syncthreads();
    }
    
    template<int NumBins, int NumThreads>
    __device__ void rearrange(DataInterface data, int * smem)
    {
        int iRoot = smem[0];
        int2 root = data.nodes[iRoot];
        int nbatch = numBatches<NumThreads>(root);
        if(nbatch>1) 
            rearrangeBatched<NumBins, NumThreads>(root, nbatch, data, smem);
        else 
            rearrangeInBlock<NumBins, NumThreads>(root, data, smem);
    }
    
    template<int NumBins, int NumThreads>
    __device__ void rearrangeInBlock(int2 root, DataInterface data, int * smem)   
    {
/*
 *    layout of memory in int
 *    t  as num threads
 *    16 as size of bin
 *
 *    0                                          workId
 *    1                -> 1+1*16-1               split bin
 *    1+1*16           -> 1+1*16+t*2-1           sides
 *    1+1*16+t*2       -> 1+1*16+t*2+2-1         group begin
 *    1+1*16+t*2+2     -> 1+1*16+t*2+2+t*2-1     offsets
 *    1+1*16+t*2+2+t*2 -> 1+1*16+t*2+2+t*2+t*2-1 backup indirection
 *    
 */         
        KeyValuePair * major = data.primitiveIndirections;
        KeyValuePair * backup = (KeyValuePair *)&smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + NumThreads * 2
                                        + 2
                                        + NumThreads * 2];
        Aabb * boxes = data.primitiveAabbs;
        
        const int j = root.x + threadIdx.x;
            
        if(j <= root.y) 
            backup[threadIdx.x] = major[j];
        
        __syncthreads();
        
        SplitBin * sSplit = (SplitBin *)&smem[1];
        float splitPlane = sSplit->plane;
        int splitDimension = sSplit->dimension;
        
        int * groupBegin = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                    + NumThreads * 2];
                                    
        if(threadIdx.x == 0)
            groupBegin[threadIdx.x] = root.x;
            
        if(threadIdx.x == 1)
            groupBegin[threadIdx.x] = root.x + sSplit->leftCount;
        
        __syncthreads();
        
        int * sSide = &smem[1 + SIZE_OF_SPLITBIN_IN_INT];
        int * sOffset = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + NumThreads * 2
                                        + 2];
            
        int * sideVertical = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + threadIdx.x * 2];
                                        
        int * offsetVertical = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + NumThreads * 2
                                        + 2
                                        + threadIdx.x * 2];
                                        
        int splitSide, ind;

        sideVertical[0] = 0;
        sideVertical[1] = 0;
        
        __syncthreads();
        
        if(j<= root.y) {
            splitSide = computeSplitSide(boxes[backup[threadIdx.x].value], splitPlane, splitDimension);
            sideVertical[splitSide]++;
        }
            
        __syncthreads();
            
        onebitsort::scanInBlock<int>(sOffset, sSide);
            
        if(j<= root.y) {
            ind = groupBegin[splitSide] + offsetVertical[splitSide];
            major[ind] = backup[threadIdx.x];
// for debug purpose only
            // major[ind].key = splitSide;
        }
    }

    template<int NumBins, int NumThreads>
    __device__ void rearrangeBatched(int2 root, int nbatch, DataInterface data, int * smem)   
    {

        KeyValuePair * major = data.primitiveIndirections;
        KeyValuePair * backup = data.intermediateIndirections;
        Aabb * boxes = data.primitiveAabbs;
        
        int i=0;
        for(;i<nbatch;i++)
            writeIndirection(backup, major, root.x + i*NumThreads, root.y);
        
        __syncthreads();
        
/*
 *    layout of memory in int
 *    t  as num threads
 *    16 as size of bin
 *
 *    0                                      workId
 *    1             -> 1+1*16-1              split bin
 *    1+1*16        -> 1+1*16+t*2-1          sides
 *    1+1*16+t*2    -> 1+1*16+t*2+2-1        group begin
 *    1+1*16+t*2+2  -> 1+1*16+t*2+2+t*2-1    offsets
 *
 *    layout of sides
 *
 *    0      1      2        n-1     thread 
 * 
 *    2*0    2*1    2*2      2*(n-1)
 *    2*1-1  2*2-1  2*3-1    2*n-1
 */        
 
        SplitBin * sSplit = (SplitBin *)&smem[1];
        float splitPlane = sSplit->plane;
        int splitDimension = sSplit->dimension;
        
        int * groupBegin = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                    + NumThreads * 2];
                                    
        if(threadIdx.x == 0)
            groupBegin[threadIdx.x] = root.x;
            
        if(threadIdx.x == 1)
            groupBegin[threadIdx.x] = root.x + sSplit->leftCount;
        
        __syncthreads();
        
        int * sSide = &smem[1 + SIZE_OF_SPLITBIN_IN_INT];
        int * sOffset = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + NumThreads * 2
                                        + 2];
            
        int * sideVertical = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + threadIdx.x * 2];
                                        
        int * offsetVertical = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + NumThreads * 2
                                        + 2
                                        + threadIdx.x * 2];
                                        
        int * sideHorizontal = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + threadIdx.x];
                                        
        int * offsetHorizontal = &smem[1 + SIZE_OF_SPLITBIN_IN_INT
                                        + NumThreads * 2
                                        + 2
                                        + threadIdx.x];
        int j, splitSide, ind;
        for(i=0;i<nbatch;i++) {
            sideVertical[0] = 0;
            sideVertical[1] = 0;
            
            __syncthreads();
            
            j = root.x + i*NumThreads + threadIdx.x;
            if(j<= root.y) {
                splitSide = computeSplitSide(boxes[backup[j].value], splitPlane, splitDimension);
                sideVertical[splitSide]++;
            }
            
            __syncthreads();
            
            onebitsort::scanInBlock<int>(sOffset, sSide);
            
            if(j<= root.y) {
                ind = groupBegin[splitSide] + offsetVertical[splitSide];
                major[ind] = backup[j];
// for debug purpose only
                // major[ind].key = splitSide;
            }
            __syncthreads();
            
            if(threadIdx.x < 2) {
                groupBegin[threadIdx.x] += sideHorizontal[2*(NumThreads-1)]
                                        + offsetHorizontal[2*(NumThreads-1)];
            }
            __syncthreads();
        }
        
    }
    
    template <typename QueueType>
    __device__ void spawn(QueueType * q, DataInterface data, int * smem)
    {
        int & iRoot = smem[0];
        int2 root = data.nodes[iRoot];
        
        SplitBin * sBestBin = (SplitBin *)&smem[1];
        int headToSecond = root.x + sBestBin->leftCount;
        const int child = q->enqueue2();
        data.nodes[child].x = root.x;
        data.nodes[child].y = headToSecond - 1;
        data.nodeAabbs[child] = sBestBin->leftBox;
        
        data.nodes[child+1].x = headToSecond;
        data.nodes[child+1].y = root.y;
        data.nodeAabbs[child+1] = sBestBin->rightBox;
        
        int2 childInd;
        childInd.x = (child | 0x80000000);
        childInd.y = ((child+1) | 0x80000000);
        data.nodes[iRoot] = childInd;
        
        data.nodeParents[child] = iRoot;
        data.nodeParents[child+1] = iRoot;
        
        const int level = data.nodeLevels[iRoot] + 1; 
        data.nodeLevels[child] = level;
        data.nodeLevels[child+1] = level;
    }
    
    __device__ int validateSplit(void * smem)
    {
        SplitBin * sBestBin = (SplitBin *)smem;
// for a valid split
        return (sBestBin[0].leftCount > 0 && sBestBin[0].rightCount > 0);
    }
    
    template<int NumThreads>
    __device__ int numBatches(int2 range)
    {
        int nbatch = (range.y - range.x + 1)/NumThreads;
        if((( range.y - range.x + 1) & (NumThreads-1)) > 0) nbatch++;
        return nbatch;
    }
};

template <typename QueueType, typename TaskType, typename TaskData, int IdelLimit, int NumBins, int NumThreads>
__global__ void work_kernel(QueueType * q,
                        TaskType task,
                        TaskData data,
                        int loopLimit, 
                        int workLimit)
{
    extern __shared__ int smem[]; 
    
    int & sWorkPerBlock = smem[0];
    
    int i;

    for(i=0;i<loopLimit;i++) {
        if(q->template isDone<IdelLimit>(workLimit)) break;
        
        if(threadIdx.x == 0) {
            sWorkPerBlock = q->dequeue();
        }     
        __syncthreads();
        
        if(sWorkPerBlock>-1) {
            task.template execute<QueueType, NumBins, NumThreads>(q, data, smem);
        } else {
            q->advanceStopClock();
            i--;
        } 
    }
}

__global__ void initHash_kernel(KeyValuePair * primitiveIndirections,
                    uint n)
{
    uint ind = blockIdx.x*blockDim.x + threadIdx.x;
	if(ind >= n) return;
	
	primitiveIndirections[ind].value = ind;
}

}
