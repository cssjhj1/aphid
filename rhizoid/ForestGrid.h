/*
 *  ForestGrid.h
 *  proxyPaint
 *
 *  Created by jian zhang on 2/18/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef APH_RHIZOID_FOREST_GRID_H
#define APH_RHIZOID_FOREST_GRID_H

#include <sdb/WorldGrid.h>
#include <kd/ClosestToPointEngine.h>
#include "ForestCell.h"

namespace aphid {

class ForestCell;
class Plant;

class ForestGrid : public sdb::WorldGrid<ForestCell, Plant > {

public:
	ForestGrid();
	virtual ~ForestGrid();
	
	template<typename T>
	void addCells(T * ground,
					const Vector3F & center,
					const float & radius);
	
};

template<typename T>
void ForestGrid::addCells(T * ground,
					const Vector3F & center,
					const float & radius)
{
	const Vector3F plow(center.x - radius,
						center.y - radius,
						center.z - radius);
	const Vector3F phigh(center.x + radius,
						center.y + radius,
						center.z + radius);			
	const sdb::Coord3 lc = gridCoord((const float *)&plow);
	const sdb::Coord3 hc = gridCoord((const float *)&phigh);
	
	const int dim = (hc.x - lc.x) + 1;
	int i, j, k;
	sdb::Coord3 sc;
	BoundingBox cbx;
	for(k=0; k<dim;++k) {
		sc.z = lc.z + k;
		for(j=0; j<dim;++j) {
			sc.y = lc.y + j;
			for(i=0; i<dim;++i) {
				sc.x = lc.x + i;
				
				cbx = coordToGridBBox(sc);
				
				if(!ground->select(cbx) ) {
					continue;
				}
				
				ForestCell * cell = findCell(sc);
				if(!cell) { 
					cell = insertCell(sc);
					cell->buildSamples(ground);
				}
				
			}
		}
	}
	
}

}
#endif