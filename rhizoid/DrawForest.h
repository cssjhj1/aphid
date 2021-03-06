/*
 *  DrawForest.h
 *  proxyPaint
 *
 *  Created by jian zhang on 1/30/16.
 *  Copyright 2016 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "ModifyForest.h"
#include <ViewObscureCull.h>
#include <ogl/DrawBox.h>
#include <ogl/DrawInstance.h>
#include <ogl/DrawCircle.h>

namespace aphid {

class ExampVox;
class ForestCell;
class RotationHandle;
class TranslationHandle;
class ScalingHandle;
class GrowOption;

class DrawForest : public ModifyForest, public ViewObscureCull, 
public DrawBox, public DrawCircle, public DrawInstance
{
	
    Matrix44F m_useMat;
	Matrix44F m_rotMat;
	RotationHandle * m_rotHand;
    TranslationHandle * m_transHand;
	ScalingHandle * m_scalHand;
    float m_wireColor[3];
	float m_transbuf[16];
    float m_showVoxLodThresold;
    bool m_enabled;
	bool m_overrideWireColor;
	
public:
    DrawForest();
    virtual ~DrawForest();
    
/// manipulate 
	void updateManipulateSpace(GrowOption & option);
    void startRotate(const Ray & r);
    void processRotate(const Ray & r);
    void finishRotate();
    void startTranslate(const Ray & r);
    void processTranslate(const Ray & r);
    void finishTranslate();
	void startResize(const Ray & r);
    void processResize(const Ray & r);
    void finishResize();
    
protected:
	void setOvrrideWireColor(bool x);
	void setScaleMuliplier(float x, float y, float z);
    float plantExtent(int idx) const;
	void drawSolidPlants();
	void drawSolidPlants2();
	void drawWiredPlants();
	void drawGridBounding();
	void drawGrid();
	void drawSample();
	void drawActiveSamples();
	void drawActivePlantContour();
	void drawActivePlants();
	void drawViewFrustum();
	bool isVisibleInView(Plant * pl, 
					const ExampVox * v,
					const float lowLod, const float highLod);
	void setShowVoxLodThresold(const float & x);
    void drawBrush();
	void setWireColor(const float & r, const float & g, const float & b);
    void enableDrawing();
    void disableDrawing();
	void drawManipulator();
    
	virtual void getDeltaRotation(Matrix33F & mat,
					const float & weight = 1.f) const;
    virtual void getDeltaTranslation(Vector3F & vec,
					const float & weight = 1.f) const;
	virtual void getDeltaScaling(Vector3F & vec,
					const float & weight = 1.f) const;
                    
private:
    void drawPlantsInCell(ForestCell * cell,
					const BoundingBox & box);
	void drawPlant(PlantData * data,
					const ExampVox * v);
	void drawWiredPlantsInCell(ForestCell * cell);
	void drawWiredPlant(PlantData * data,
					const ExampVox * v);
	void drawLODPlant(PlantData * data,
					const ExampVox * v);
	void drawPlantSolidBoundInCell(ForestCell * cell);
	void drawPlantSolidBound(PlantData * data,
					const ExampVox * v);
	void drawPlantFlatSolidBound(PlantData * data,
					const ExampVox * v);
					
};

}