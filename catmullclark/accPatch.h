/*
 *  accPatch.h
 *  catmullclark
 *
 *  Created by jian zhang on 10/28/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include <bezierPatch.h>

class AccStencil;
class AccPatch : public BezierPatch {
public:
	AccPatch();
	~AccPatch();
	virtual void evaluateContolPoints();
	
	void processCornerControlPoints(int i);
	void processEdgeControlPoints(int i);
	void processInteriorControlPoints(int i);
	
	static AccStencil* stencil;
};
