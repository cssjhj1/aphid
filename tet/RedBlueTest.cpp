/*
 *  RedBlueTest.cpp
 *  foo
 *
 *  Created by jian zhang on 7/14/16.
 *  Copyright 2016 __MyCompanyName__. All rights reserved.
 *
 */

#include "RedBlueTest.h"
#include <iostream>
#include <tetrahedron_math.h>

using namespace aphid;
namespace ttg {

RedBlueTest::RedBlueTest() 
{}

RedBlueTest::~RedBlueTest() 
{}
	
const char * RedBlueTest::titleStr() const
{ return "Red Blue Refine Test"; }

bool RedBlueTest::init()
{
	const float l = 10.f;
	m_nodeDrawSize = l * .125f;
	
	m_p[0].set(-l, -.5f * l, 0.f);
	m_p[1].set( l, -.5f * l, 0.f);
	m_p[2].set(0.f, .5f * l, -l);
	m_p[3].set(0.f, .5f * l,  l);
	m_d[0] = m_d[1] = m_d[2] = m_d[3] = 1.f;
	
	doRefine();
	return true;
}

void RedBlueTest::doRefine()
{
	std::cout<<"\n refine "<<m_d[0]<<", "<<m_d[1]<<", "<<m_d[2]<<", "<<m_d[3];
	
	m_rbr.set(0, 1, 2, 3);
	
	m_rbr.evaluateDistance(m_d[0], m_d[1], m_d[2], m_d[3]);
	
	int auv = 4;
	if(m_rbr.needSplitRedEdge(0) ) {
		m_p[auv] = m_rbr.splitPos(m_d[0], m_d[1], m_p[0], m_p[1]);
		m_rbr.splitRedEdge(0, auv++);
	}
	
	if(m_rbr.needSplitRedEdge(1) ) {
		m_p[auv] = m_rbr.splitPos(m_d[2], m_d[3], m_p[2], m_p[3]);
		m_rbr.splitRedEdge(1, auv++);
	}
	
	if(m_rbr.needSplitBlueEdge(0) ) {
		m_p[auv] = m_rbr.splitPos(m_d[0], m_d[2], m_p[0], m_p[2]);
		m_rbr.splitBlueEdge(0, auv++);
	}
	
	if(m_rbr.needSplitBlueEdge(1) ) {
		m_p[auv] = m_rbr.splitPos(m_d[0], m_d[3], m_p[0], m_p[3]);
		m_rbr.splitBlueEdge(1, auv++);
	}
	
	if(m_rbr.needSplitBlueEdge(2) ) {
		m_p[auv] = m_rbr.splitPos(m_d[1], m_d[2], m_p[1], m_p[2]);
		m_rbr.splitBlueEdge(2, auv++);
	}
	
	if(m_rbr.needSplitBlueEdge(3) ) {
		m_p[auv] = m_rbr.splitPos(m_d[1], m_d[3], m_p[1], m_p[3]);
		m_rbr.splitBlueEdge(3, auv++);
	}
	
	m_rbr.refine();
	m_rbr.verbose();
	checkTetraVolume();
}

void RedBlueTest::setA(double x)
{
	m_d[0] = x;
	doRefine();
}

void RedBlueTest::setB(double x)
{
	m_d[1] = x;
	doRefine();
}

void RedBlueTest::setC(double x)
{
	m_d[2] = x;
	doRefine();
}

void RedBlueTest::setD(double x)
{
	m_d[3] = x;
	doRefine();
}

void RedBlueTest::checkTetraVolume()
{
	float mnvol = 1e20f, mxvol = -1e20f, vol;
	Vector3F p[4];
	const int n = m_rbr.numTetra();
	int i = 0;
	for(;i<n;++i) {
		const ITetrahedron * t = m_rbr.tetra(i);
		
		p[0] = m_p[t->iv0];
		p[1] = m_p[t->iv1];
		p[2] = m_p[t->iv2];
		p[3] = m_p[t->iv3];
		
		vol = tetrahedronVolume(p);
		if(mnvol > vol)
			mnvol = vol;
		if(mxvol < vol)
			mxvol = vol;
			
	}

	std::cout<<"\n min/max tetrahedron volume: "<<mnvol<<" / "<<mxvol;
	if(mnvol < 0.f)
		std::cout<<"\n [ERROR] negative volume";
		
}

void RedBlueTest::draw(aphid::GeoDrawer * dr)
{
	dr->setColor(.3f, .3f, .4f);
	const int nt = m_rbr.numTetra();
	int i=0;
	for(;i<nt;++i) {
		const ITetrahedron * t = m_rbr.tetra(i);
		
		Vector3F a = m_p[t->iv0];
		Vector3F b = m_p[t->iv1];
		Vector3F c = m_p[t->iv2];
		Vector3F d = m_p[t->iv3];
		dr->tetrahedronWire(a, b, c, d);
	}
}

}