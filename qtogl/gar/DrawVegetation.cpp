/*
 *  DrawVegetation.cpp
 *  
 *
 *  Created by jian zhang on 4/19/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#include "DrawVegetation.h"
#include <geom/ATriangleMesh.h>
#include "PlantPiece.h"
#include <gl_heads.h>

using namespace aphid;

DrawVegetation::DrawVegetation()
{}

DrawVegetation::~DrawVegetation()
{}

void DrawVegetation::drawPlant(const PlantPiece * pl)
{
	drawPiece(pl);
}

void DrawVegetation::drawPiece(const PlantPiece * pl)
{
	glPushMatrix();
	float transbuf[16];
	const Matrix44F & tm = pl->transformMatrix();
	tm.glMatrix(transbuf);
	glMultMatrixf((const GLfloat*)transbuf);
	
	const ATriangleMesh * geo = pl->geometry();
	drawMesh(geo);
	
	const int n = pl->numBranches();
	for(int i=0;i<n;++i) {
		drawPiece(pl->branch(i) );
	}
	glPopMatrix();
}

void DrawVegetation::drawMesh(const ATriangleMesh * geo)
{
	if(!geo) {
		return;
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glNormalPointer(GL_FLOAT, 0, (const GLfloat*)geo->vertexNormals() );
	glVertexPointer(3, GL_FLOAT, 0, (const GLfloat*)geo->points() );
	glDrawElements(GL_TRIANGLES, geo->numIndices(), GL_UNSIGNED_INT, geo->indices() );
	
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}