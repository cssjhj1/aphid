/*
 *  ExampVox.h
 *  proxyPaint
 *
 *  Created by jian zhang on 2/5/16.
 *  Copyright 2016 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef APH_EXAMP_VOX_H
#define APH_EXAMP_VOX_H

#include <ogl/DrawBox.h>
#include <ogl/DrawDop.h>
#include <ogl/DrawTriangle.h>
#include <math/BoundingBox.h>

namespace aphid {
 
namespace sdb {

template<typename T>
class VectorArray;

}
    
namespace cvx {

class Triangle;

}

class ExampVox : public DrawBox, public DrawDop, public DrawTriangle {

	BoundingBox m_geomBox;
	Vector3F m_geomCenter;
	Vector3F m_dopSize;
	
	float m_diffuseMaterialColV[3];
/// radius of bbox
	float m_geomExtent;
/// radius of exclusion
	float m_geomSize;
/// scaling radius of exclusion
	float m_sizeMult;
	
public:
	ExampVox();
	virtual ~ExampVox();
	
	virtual void voxelize2(sdb::VectorArray<cvx::Triangle> * tri,
							const BoundingBox & bbox);
							
	virtual void voxelize3(sdb::VectorArray<cvx::Triangle> * tri,
							const BoundingBox & bbox);
	
/// set b4 geom box
	void setGeomSizeMult(const float & x);
	void setGeomBox(BoundingBox * bx);
	void setDopSize(const float & a,
	                const float & b,
	                const float &c);
	
	const float & geomExtent() const;
	const float & geomSize() const;
	const BoundingBox & geomBox() const;
	const Vector3F & geomCenter() const;
	const float * diffuseMaterialColor() const;
	const float * dopSize() const;
	
	virtual void drawWiredBound() const;
	virtual void drawSolidBound() const;
	
	virtual int numExamples() const;
	virtual int numInstances() const;
	virtual const ExampVox * getExample(const int & i) const;
	virtual ExampVox * getExample(const int & i);
	
	struct InstanceD {
		float _trans[16];
		int _exampleId;
		int _instanceId;
	};
	
	virtual const InstanceD & getInstance(const int & i) const;
	
protected:
	float * diffuseMaterialColV();
	void buildBounding8Dop(const BoundingBox & bbox);
						
private:
	InstanceD m_defInstance;
	
};

}
#endif
