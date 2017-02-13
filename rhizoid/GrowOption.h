/*
 *  GrowOption.h
 *  proxyPaint
 *
 *  Created by jian zhang on 2/14/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef APH_RHIZ_GROW_OPTION_H
#define APH_RHIZ_GROW_OPTION_H

#include <math/Vector3F.h>

namespace aphid {

class ExrImage;

class GrowOption {

	ExrImage * m_sampler;

public:
	Vector3F m_upDirection;
	Vector3F m_centerPoint;
	Vector3F m_noiseOrigin;
	int m_plantId;
	float m_minScale, m_maxScale;
	float m_minMarginSize, m_maxMarginSize;
	float m_rotateNoise;
	float m_strength;
	float m_radius;
	float m_noiseFrequency;
	float m_noiseLacunarity;
	float m_noiseLevel;
	float m_noiseGain;
	int m_noiseOctave;
	bool m_alongNormal;
	bool m_multiGrow;
	bool m_stickToGround;
	bool m_isInjectingParticle;
	float m_strokeMagnitude;
	
	GrowOption();
	~GrowOption();
	
	void setStrokeMagnitude(const float & x);
	bool openImage(const std::string & fileName);
	bool hasSampler() const;
	
};

}
#endif
