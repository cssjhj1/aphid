TEMPLATE = lib
DESTDIR = ./
CONFIG += staticlib release
CONFIG   -= qt
PARENTPROJ = ..
SHAREDDIR = ../shared
INCLUDEPATH = $${SHAREDDIR}

INCLUDEPATH += ../mallard ../shared ../lapl ../catmullclark ../easymodel ../../ofl/opium ../fit ../powell

HEADERS = ../shared/AllMath.h ../shared/ATypes.h ../shared/AllLight.h \
                ../shared/Vector3F.h \
                ../shared/Vector2F.h \
                ../shared/Matrix44F.h \
                ../shared/Matrix33F.h \
                ../shared/Quaternion.h \
                ../shared/Ray.h \
                ../shared/RayIntersectionContext.h \
                ../shared/IntersectionContext.h \
                ../shared/BaseCamera.h \
                ../shared/PerspectiveCamera.h \
                ../shared/BaseDrawer.h \
                ../shared/KdTreeDrawer.h \
                ../shared/SpaceHandle.h \
                ../shared/TypedEntity.h \
				../shared/NamedEntity.h \
				../shared/BaseTransform.h \
                ../shared/Geometry.h \
                ../shared/BaseMesh.h \
				../shared/PlaneMesh.h \
                ../shared/TriangleMesh.h \
                ../shared/Triangle.h \
                ../shared/GeoElement.h \
                ../shared/Vertex.h \
                ../shared/Edge.h \
                ../shared/Facet.h \
                ../shared/BaseField.h \
                ../shared/BaseDeformer.h \
    ../shared/SkeletonSubspaceDeformer.h \
                ../shared/KdTreeNode.h \
                ../shared/Primitive.h \
                ../shared/PrimitiveFilter.h \
                ../shared/BuildKdTreeContext.h \
                ../shared/KdTreeNodeArray.h \
                ../shared/KdTree.h \
                ../shared/PrimitiveArray.h \
                ../shared/IndexArray.h \
                ../shared/BaseArray.h \
                ../shared/ClassificationStorage.h \
                ../shared/KdTreeBuilder.h \
                ../shared/SplitEvent.h \
                ../shared/MinMaxBins.h \
                ../shared/BuildKdTreeStream.h \
                ../shared/IndexList.h \
                ../shared/BoundingBoxList.h \
                ../shared/SelectionArray.h \
                ../shared/ComponentConversion.h \
                ../shared/BarycentricCoordinate.h \
                ../shared/EasemodelUtil.h \
                ../shared/GeodesicSphereMesh.h \
                                ../shared/PyramidMesh.h \
                                ../shared/CubeMesh.h \
				../shared/DiscMesh.h \
                ../shared/AnchorGroup.h \
                ../shared/BaseCurve.h \
                ../shared/VertexPath.h \
                ../shared/MeshTopology.h \
                ../shared/PatchMesh.h \
                                ../shared/BezierCurve.h \
                                ../shared/ToolContext.h \
                                ../shared/BiLinearInterpolate.h \
                                ../shared/LinearInterpolate.h \
                ../shared/Plane.h \
                ../shared/PointInsidePolygonTest.h \
                                ../shared/AccPatchMesh.h \
								../shared/PatchNeighborRec.h \
								../shared/PatchTexture.h \
                                ../shared/CircleCurve.h \
                                ../shared/InverseBilinearInterpolate.h \
                                ../shared/Segment.h \
                                ../shared/Patch.h \
                                ../shared/BaseBrush.h \
                                ../shared/QuickSort.h \
                                ../shared/GLight.h \
                                ../shared/GMaterial.h \
                                ../shared/GProfile.h \
                                ../shared/BaseTessellator.h \
                                ../shared/CollisionRegion.h \
                                ../shared/DrawBuffer.h \
                                ../shared/BlockDrawBuffer.h \
                ../shared/BaseScene.h \
    ../shared/BoundingBox.h \
    ../shared/BoundingRectangle.h \
    ../shared/AllHdf.h \
    ../shared/HObject.h \
    ../shared/HDocument.h \
    ../shared/HGroup.h \
    ../shared/HDataset.h \
    ../shared/HAttribute.h \
    ../shared/VerticesHDataset.h \
    ../shared/IndicesHDataset.h \
    ../shared/FloatsHDataset.h \
    ../shared/HIntAttribute.h \
    ../shared/HFloatAttribute.h \
    ../shared/HStringAttribute.h \
    ../shared/HCharData.h \
    ../shared/HBase.h \
    ../shared/HWorld.h \
    ../shared/HMesh.h \
    ../shared/BaseImage.h \
    ../shared/zEXRImage.h \
    ../shared/BaseFile.h \
    ../shared/BakeDeformer.h \
    ../shared/PlaybackControl.h \
    ../shared/CacheFile.h \
    ../shared/BaseState.h \
    ../shared/HFile.h \
    ../shared/HRFile.h \
../shared/TransformManipulator.h \
../shared/MeshManipulator.h \
	../shared/SkeletonJoint.h \
	../shared/SkeletonSystem.h \
	../shared/SkeletonPose.h \
	../shared/LineBuffer.h \
	../shared/SelectCondition.h \
    ../shared/FloodCondition.h \
	../shared/ActiveRegion.h \
	../shared/AccPatchGroup.h \
	../shared/VertexDataGroup.h \
	../shared/BezierPatchHirarchy.h \
	../shared/KMeansClustering.h \
	../shared/BaseSphere.h \
	../shared/PseudoNoise.h \
	../shared/BaseServer.h \
    ../shared/BaseClient.h \
	../shared/RenderEngine.h \
	../shared/AdaptableStripeBuffer.h \
	../shared/BlockStripeBuffer.h \
	../shared/LODFn.h \
	../shared/NoisePlot.h \
	../shared/FractalPlot.h \
	../shared/BaseLight.h \
	../shared/DistantLight.h \
	../shared/PointLight.h \
	../shared/SquareLight.h \
../shared/LightGroup.h \
../shared/LineDrawer.h \
../shared/MeshDrawer.h \
../shared/GeoDrawer.h \
../shared/LightDrawer.h \
../shared/RenderOptions.h \
../shared/SelectionContext.h \
../shared/BaseTexture.h \
../shared/TextureGroup.h \
../shared/ColorBlend.h \
../shared/TexturePainter.h \
../shared/BaseShader.h \
../shared/ShaderGroup.h \
../shared/RayMarch.h \
../shared/GLHUD.h \
../shared/BaseLog.h \
                                ../lapl/VertexAdjacency.h \
                ../lapl/Anchor.h \
                ../catmullclark/BaseQuad.h \
                ../catmullclark/LODQuad.h \
                ../catmullclark/accPatch.h \
                ../catmullclark/accStencil.h \
                ../catmullclark/bezierPatch.h \
                ../catmullclark/tessellator.h \
                ../catmullclark/AccCorner.h \
                ../catmullclark/AccEdge.h \
                ../catmullclark/AccInterior.h \
                ../catmullclark/BezierDrawer.h \
				../powell/PowellMethod.h \
				../powell/BaseFunction.h
SOURCES = ../shared/Vector3F.cpp \
                ../shared/Vector2F.cpp \
                ../shared/Matrix44F.cpp \
                ../shared/Matrix33F.cpp \
                ../shared/Quaternion.cpp \
                ../shared/Ray.cpp \
                ../shared/RayIntersectionContext.cpp \
                ../shared/IntersectionContext.cpp \
                ../shared/BaseCamera.cpp \
                ../shared/PerspectiveCamera.cpp \
                ../shared/BaseDrawer.cpp \
                ../shared/KdTreeDrawer.cpp \
                ../shared/SpaceHandle.cpp \
                ../shared/TypedEntity.cpp \
				../shared/NamedEntity.cpp \
				../shared/BaseTransform.cpp \
                ../shared/Geometry.cpp \
                ../shared/BaseMesh.cpp \
				../shared/PlaneMesh.cpp \
                ../shared/TriangleMesh.cpp \
                ../shared/Triangle.cpp \
                ../shared/GeoElement.cpp \
                ../shared/Vertex.cpp \
                ../shared/Edge.cpp \
                ../shared/Facet.cpp \
                ../shared/BaseField.cpp \
                ../shared/BaseDeformer.cpp \
    ../shared/SkeletonSubspaceDeformer.cpp \
                ../shared/KdTreeNode.cpp \
                ../shared/Primitive.cpp \
                ../shared/PrimitiveFilter.cpp \
                ../shared/BuildKdTreeContext.cpp \
                ../shared/KdTreeNodeArray.cpp \
                ../shared/KdTree.cpp \
                ../shared/PrimitiveArray.cpp \
                ../shared/IndexArray.cpp \
                ../shared/BaseArray.cpp \
                ../shared/ClassificationStorage.cpp \
                ../shared/KdTreeBuilder.cpp \
                ../shared/SplitEvent.cpp \
                ../shared/MinMaxBins.cpp \
                ../shared/BuildKdTreeStream.cpp \
                ../shared/IndexList.cpp \
                ../shared/BoundingBoxList.cpp \
                ../shared/SelectionArray.cpp \
                ../shared/ComponentConversion.cpp \
                ../shared/BarycentricCoordinate.cpp \
                ../shared/EasemodelUtil.cpp \
                ../shared/GeodesicSphereMesh.cpp \
                ../shared/PyramidMesh.cpp \
                ../shared/CubeMesh.cpp \
				../shared/DiscMesh.cpp \
                ../shared/AnchorGroup.cpp \
                ../shared/BaseCurve.cpp \
                ../shared/VertexPath.cpp \
                ../shared/MeshTopology.cpp \
                ../shared/PatchMesh.cpp \
                                ../shared/BezierCurve.cpp \
                                ../shared/ToolContext.cpp \
                                ../shared/BiLinearInterpolate.cpp \
                                ../shared/LinearInterpolate.cpp \
                ../shared/Plane.cpp \
                ../shared/PointInsidePolygonTest.cpp \
                                ../shared/AccPatchMesh.cpp \
								../shared/PatchNeighborRec.cpp \
								../shared/PatchTexture.cpp \
                                ../shared/CircleCurve.cpp \
                                ../shared/InverseBilinearInterpolate.cpp \
                                ../shared/Segment.cpp \
                                ../shared/Patch.cpp \
                                ../shared/BaseBrush.cpp \
                                ../shared/QuickSort.cpp \
                                ../shared/GLight.cpp \
                                ../shared/GMaterial.cpp \
                                ../shared/GProfile.cpp \
                                ../shared/BaseTessellator.cpp \
                                ../shared/CollisionRegion.cpp \
                                ../shared/DrawBuffer.cpp \
                                ../shared/BlockDrawBuffer.cpp \
                ../shared/BaseScene.cpp \
    ../shared/BoundingBox.cpp \
    ../shared/BoundingRectangle.cpp \
    ../shared/HObject.cpp \
    ../shared/HDocument.cpp \
    ../shared/HGroup.cpp \
    ../shared/HDataset.cpp \
    ../shared/HAttribute.cpp \
    ../shared/VerticesHDataset.cpp \
    ../shared/IndicesHDataset.cpp \
        ../shared/FloatsHDataset.cpp \
    ../shared/HIntAttribute.cpp \
    ../shared/HFloatAttribute.cpp \
    ../shared/HStringAttribute.cpp \
    ../shared/HCharData.cpp \
    ../shared/HBase.cpp \
    ../shared/HWorld.cpp \
    ../shared/HMesh.cpp \
    ../shared/BaseImage.cpp \
    ../shared/zEXRImage.cpp \
        ../shared/BaseFile.cpp \
    ../shared/BakeDeformer.cpp \
    ../shared/PlaybackControl.cpp \
    ../shared/CacheFile.cpp \
    ../shared/BaseState.cpp \
    ../shared/HFile.cpp \
    ../shared/HRFile.cpp \
    ../shared/TransformManipulator.cpp \
    ../shared/MeshManipulator.cpp \
	../shared/SkeletonJoint.cpp \
	../shared/SkeletonSystem.cpp \
	../shared/SkeletonPose.cpp \
	../shared/LineBuffer.cpp \
	../shared/SelectCondition.cpp \
    ../shared/FloodCondition.cpp \
	../shared/ActiveRegion.cpp \
	../shared/AccPatchGroup.cpp \
	../shared/VertexDataGroup.cpp \
	../shared/BezierPatchHirarchy.cpp \
	../shared/KMeansClustering.cpp \
	../shared/BaseSphere.cpp \
	../shared/PseudoNoise.cpp \
	../shared/BaseServer.cpp \
    ../shared/BaseClient.cpp \
	../shared/RenderEngine.cpp \
	../shared/AdaptableStripeBuffer.cpp \
	../shared/BlockStripeBuffer.cpp \
	../shared/LODFn.cpp \
	../shared/NoisePlot.cpp \
	../shared/FractalPlot.cpp \
	../shared/BaseLight.cpp \
	../shared/DistantLight.cpp \
	../shared/PointLight.cpp \
	../shared/SquareLight.cpp \
../shared/LightGroup.cpp \
../shared/LineDrawer.cpp \
../shared/MeshDrawer.cpp \
../shared/GeoDrawer.cpp \
../shared/LightDrawer.cpp \
../shared/RenderOptions.cpp \
../shared/SelectionContext.cpp \
../shared/BaseTexture.cpp \
../shared/TextureGroup.cpp \
../shared/ColorBlend.cpp \
../shared/TexturePainter.cpp \
../shared/BaseShader.cpp \
../shared/ShaderGroup.cpp \
../shared/RayMarch.cpp \
../shared/GLHUD.cpp \
../shared/BaseLog.cpp \
                                ../lapl/VertexAdjacency.cpp \
                ../lapl/Anchor.cpp \
                ../catmullclark/BaseQuad.cpp \
                ../catmullclark/LODQuad.cpp \
                ../catmullclark/accPatch.cpp \
                ../catmullclark/accStencil.cpp \
                ../catmullclark/bezierPatch.cpp \
                ../catmullclark/tessellator.cpp \
                ../catmullclark/AccCorner.cpp \
                ../catmullclark/AccEdge.cpp \
                ../catmullclark/AccInterior.cpp \
                ../catmullclark/BezierDrawer.cpp \
				../powell/PowellMethod.cpp \
				../powell/BaseFunction.cpp

TARGET  = aphid
macx {
    INCLUDEPATH += ../../Library/boost_1_55_0 \
                    ../../Library/hdf5/include \
                    /usr/local/include/OpenEXR
}
win32 {
    HEADERS += ../shared/gExtension.h
    SOURCES += ../shared/gExtension.cpp
    INCLUDEPATH += D:/usr/boost_1_51_0 \
                    D:/usr/openEXR/include \
                    D:/ofl/shared \
                   D:/usr/hdf5/include \
                   D:/usr/libxml2/include \
                   D:/usr/eigen3
    DEFINES += OPENEXR_DLL NDEBUG _WIN32_WINDOWS
}
