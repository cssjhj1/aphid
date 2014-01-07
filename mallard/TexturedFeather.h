/*
 *  TexturedFeather.h
 *  mallard
 *
 *  Created by jian zhang on 12/24/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "BaseFeather.h"
#include <zEXRImage.h>
class AdaptableStripeBuffer;
class BaseVane;
class TexturedFeather : public BaseFeather {
public:
	TexturedFeather();
	virtual ~TexturedFeather();
	virtual void computeTexcoord();
	virtual void translateUV(const Vector2F & d);
	
	BaseVane * uvVane(short side) const;
	Vector3F * uvVaneCoord(short u, short v, short side);
	
	AdaptableStripeBuffer * stripe();
	static ZEXRImage ColorTextureFile;
	
	void setResShaft(unsigned resShaft);
	void setResBarb(unsigned resBarb);
	unsigned resShaft() const;
	unsigned resBarb() const;
	
	void sampleColor(unsigned gridU, unsigned gridV, Vector3F * dst);
	void sampleColor(float lod);
	void sampleColor(unsigned nu, unsigned nv, int side);
	
	unsigned numStripe() const;
	unsigned numStripePoints() const;
protected:

private:
	BaseVane * m_vane;
	AdaptableStripeBuffer * m_stripe;
	unsigned m_resShaft, m_resBarb;
};