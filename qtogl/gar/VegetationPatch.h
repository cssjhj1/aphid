/*
 *  VegetationPatch.h
 *  garden
 *
 *  grow a patch via dart throwing around origin
 *
 *  Created by jian zhang on 4/19/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAR_VEGETATION_PATCH_H
#define GAR_VEGETATION_PATCH_H

#include <vector>

namespace aphid {
class Vector3F;
class Matrix44F;
class BoundingBox;
}

class PlantPiece;

class VegetationPatch {

	typedef std::vector<PlantPiece *> PlantListTyp;
/// to roots
	PlantListTyp m_plants;
	float m_tmv[16];
	float m_yardR;
	float m_tilt;
	
public:
	VegetationPatch();
	virtual ~VegetationPatch();
	
	int numPlants() const;
	const PlantPiece * plant(const int & i) const;
	
/// cannot collide with existing ones
	bool addPlant(PlantPiece * pl);
	void clearPlants();
	
	bool isFull() const;
	
	void setTilt(const float & x);
	const float & tilt() const;
	
	const float & yardRadius() const;
	
	void setTransformation(const aphid::Matrix44F & tm);

	const float * transformationV() const;
	
	int getNumTms();
	void extractTms(aphid::Matrix44F * dst);
	
	void getBBox(aphid::BoundingBox * dst);
	
protected:

private:
	bool intersectPlants(const aphid::Vector3F & pos, 
		const float & r) const;
	
};

#endif