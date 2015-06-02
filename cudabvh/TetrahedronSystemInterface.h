#ifndef TETRAHEDRONSYSTEMINTERFACE_H
#define TETRAHEDRONSYSTEMINTERFACE_H

#include "bvh_common.h"
#define TETRAHEDRONSYSTEM_VICINITY_LENGTH 32

namespace tetrasys {
    
void writeVicinity(int * vicinities,
                    int * indices,
                    int * offsets,
                    uint n);

void formTetrahedronAabbs(Aabb *dst, 
                        float3 * pos, 
                        float3 * vel, 
                        float timeStep, 
                        uint4 * tets, 
                        unsigned numTetrahedrons);

}
#endif        //  #ifndef TETRAHEDRONSYSTEMINTERFACE_H

