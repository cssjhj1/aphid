#include <DynGlobal.h>
class GeoDrawer;
class CudaLinearBvh;
class WorldDbgDraw {
public:
    WorldDbgDraw(GeoDrawer * drawer);
    virtual ~WorldDbgDraw();
    
#if DRAW_BVH_HASH
    void showBvhHash(CudaLinearBvh * bvh);
#endif

#if DRAW_BVH_HIERARCHY
    void showBvhHierarchy(CudaLinearBvh * bvh);
#endif

    static int MaxDrawBvhHierarchyLevel;
protected:

private:
    GeoDrawer * m_drawer;
};
