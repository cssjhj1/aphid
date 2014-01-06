#include "MlFeather.h"
#include "MlRachis.h"
#include "MlVane.h"
#include <CollisionRegion.h>
#include <AdaptableStripeBuffer.h>

MlFeather::MlFeather()
{
	m_rachis = new MlRachis;
	m_vane = new MlVane[2];
	
	simpleCreate();
	
	setSeed(1);
	setNumSeparate(2);
	setSeparateStrength(0.f);
	setFuzzy(0.f);
	m_scale = 1.f;
}

MlFeather::~MlFeather() 
{
	delete m_rachis;
	delete[] m_vane;
}

void MlFeather::createNumSegment(short x)
{
	BaseFeather::createNumSegment(x);
	m_rachis->create(x);
}

void MlFeather::setupVane()
{
	m_vane[0].create(numSegment(), 3);
	m_vane[1].create(numSegment(), 3);
	Vector3F oriP(4.f, 0.f, 4.f);
	Matrix33F oriR; oriR.fill(Vector3F::ZAxis, Vector3F::XAxis, Vector3F::YAxis);
	float sc = 1.f;
	m_rachis->reset();
	computeWorldP(oriP, oriR, sc);
	m_vane[0].computeKnots();
	m_vane[1].computeKnots();
}

void MlFeather::bend()
{
	m_rachis->bend();
}

void MlFeather::bendAt(unsigned faceIdx, float patchU, float patchV, const Vector3F & oriPos, const Matrix33F & oriRot, const float & scale)
{
	m_rachis->bend(faceIdx, patchU, patchV, oriPos, oriRot, scale * shaftLength(), m_skin);
}

void MlFeather::curl(float val)
{
	m_rachis->curl(val);
}

void MlFeather::computeWorldP(const Vector3F & oriPos, const Matrix33F & oriRot, const float & scale)
{
	m_scale = scale;
	Vector3F segOrigin = oriPos;
	Matrix33F segSpace = oriRot;
	const short numSeg = numSegment();
	for(short i = 0; i < numSeg; i++) {
		Matrix33F mat = m_rachis->getSpace(i);
		mat.multiply(segSpace);
		
		*segmentVaneWP(i, 0, 0) = segOrigin;
		*segmentVaneWP(i, 0, 1) = segOrigin;
		computeVaneWP(segOrigin, mat, i, 0, scale);
		computeVaneWP(segOrigin, mat, i, 1, scale);
		
		Vector3F d(0.f, 0.f, quilly()[i] * scale);
		d = mat.transform(d);
		
		segOrigin += d;
		segSpace = mat;
	}
	
	*segmentVaneWP(numSeg, 0, 0) = segOrigin;
	*segmentVaneWP(numSeg, 0, 1) = segOrigin;
	computeVaneWP(segOrigin, segSpace, numSeg, 0, scale);
	computeVaneWP(segOrigin, segSpace, numSeg, 1, scale);
}

Vector3F * MlFeather::segmentVaneWP(short seg, unsigned end, short side)
{
	return m_vane[side].railCV(seg, end);
}

void MlFeather::computeVaneWP(const Vector3F & origin, const Matrix33F& space, short seg, short side, float scale)
{
	Vector3F p = origin;
	Vector2F * vane = getUvDisplaceAt(seg, side);
	
	const float tapper = width() * -.005f;
	for(short i = 0; i < 3; i++) {
		Vector3F d(tapper, vane->x, vane->y);
		d *= scale;
		d = space.transform(d);
		
		p += d;
		*segmentVaneWP(seg, i+1, side) = p;
		
		vane++;
	}
}

void MlFeather::setCollision(CollisionRegion * skin)
{
	m_skin = skin;
}

void MlFeather::setFeatherId(short x)
{
	m_id = x;
}
	
short MlFeather::featherId() const
{
	return m_id;
}

void MlFeather::simpleCreate(int ns)
{
	BaseFeather::simpleCreate(ns);
	setupVane();
}

void MlFeather::computeLength()
{
	BaseFeather::computeLength();
	m_rachis->computeLengths(quilly(), shaftLength());
}

void MlFeather::changeNumSegment(int d)
{
	BaseFeather::changeNumSegment(d);
	setupVane();
}

void MlFeather::getBoundingBox(BoundingBox & box)
{
	
	for(short i = 0; i <= numSegment(); i++) {
		box.update(*segmentVaneWP(i, 3, 0));
		box.update(*segmentVaneWP(i, 1, 0));
	}
}

float * MlFeather::angles() const
{
	return m_rachis->angles();
}

float MlFeather::bendDirection() const
{
	return m_rachis->bendDirection();
}

MlVane * MlFeather::vane(short side) const
{
	return &m_vane[side];
}

void MlFeather::verbose()
{
	std::cout<<"feather index:\n id "<<featherId();
	std::cout<<"\n n segment "<<numSegment();
	std::cout<<"\n length "<<shaftLength();
	std::cout<<"\n base uv ("<<baseUV().x<<","<<baseUV().y<<")\n";
}

void MlFeather::samplePosition(Vector3F * dst)
{
	const unsigned gridU = resShaft();
	const unsigned gridV = resBarb();
	const float du = 1.f/(float)gridU;
	const float dv = 1.f/(float)gridV;
	
	
	unsigned acc = 0;
	for(unsigned i = 0; i <= gridU; i++) {
		m_vane[0].setU(du*i);
		for(unsigned j = 0; j <= gridV; j++) {
			m_vane[0].pointOnVane(dv * j, dst[acc]);
			acc++;
		}
		m_vane[0].modifyLength(du*i, gridV, &dst[acc - gridV - 1], 1.f);
	}
	
	
	for(unsigned i = 0; i <= gridU; i++) {
		m_vane[1].setU(du*i);
		for(unsigned j = 0; j <= gridV; j++) {
			m_vane[1].pointOnVane(dv * j, dst[acc]);
			acc++;
		}
		m_vane[1].modifyLength(du*i, gridV, &dst[acc - gridV - 1], 1.f);
	}
}

void MlFeather::setSeed(unsigned s)
{
	m_vane[0].setSeed(s);
	m_vane[1].setSeed(s + 64873);
}

void MlFeather::setNumSeparate(unsigned n)
{
	m_vane[0].setNumSparate(n);
	m_vane[1].setNumSparate(n);
	m_numSeparate = n;
}

void MlFeather::setSeparateStrength(float k)
{
	m_vane[0].setSeparateStrength(k);
	m_vane[1].setSeparateStrength(k);
	m_separateStrength = k;
}

void MlFeather::setFuzzy(float f)
{
	m_vane[0].setFuzzy(f);
	m_vane[1].setFuzzy(f);
	m_fuzzy = f;
}

unsigned MlFeather::seed() const
{
	return m_vane[0].seed();
}

unsigned MlFeather::numSeparate() const
{
	return m_numSeparate;
}

float MlFeather::fuzzy() const
{
	return m_fuzzy;
}

float MlFeather::separateStrength() const
{
	return m_separateStrength;
}

void MlFeather::testVane()
{
	Vector3F oriP(4.f, -2.f, 4.f);
	Matrix33F oriR; oriR.fill(Vector3F::ZAxis, Vector3F::XAxis, Vector3F::YAxis);
	computeWorldP(oriP, oriR, 2.f);
	separateVane();
}

void MlFeather::separateVane()
{
	m_vane[0].separate();
	m_vane[1].separate();
}

void MlFeather::computeNoise()
{
	m_vane[0].computeNoise();
	m_vane[1].computeNoise();
}

void MlFeather::samplePosition(float lod)
{
	const unsigned nu = m_vane[0].gridU() * (2 + (resShaft() - 2) * lod);
	const unsigned nv = 3 + (resBarb() - 3) * lod;
	stripe()->begin();
	
	samplePosition(nu, nv, 0, lod);
	samplePosition(nu, nv, 1, lod);
}

void MlFeather::samplePosition(unsigned nu, unsigned nv, int side, float lod)
{
	const float rootWidth = m_scale * shaftLength() / (float)nu * .9f;
	const float tipWidth = rootWidth * 0.3f;
	const float du = 1.f/(float)nu;
	const float dv = 1.f/(float)nv;
	float shrinking, tapering = 1.f;
	for(unsigned i = 0; i < nu; i++) {
		*stripe()->currentNumCvs() = nv + 1;
		
		Vector3F * coord = stripe()->currentPos();
		float * w = stripe()->currentWidth();
		
		if(i > nu/2) tapering = 1.f - (float)(i - nu/2) / (float)nu * 1.5f;

		m_vane[side].setU(du*i);
		for(unsigned j = 0; j <= nv; j++) {
			m_vane[side].pointOnVane(dv * j, coord[j]);
			shrinking = (float)j / (float)nv;
			w[j] = (rootWidth * (1.f - shrinking) + tipWidth * shrinking) * tapering; 
		}
		m_vane[side].modifyLength(du*i, nv, coord, lod);
		stripe()->next();
	}
}

float MlFeather::scaledShaftLength() const
{
	return m_scale * shaftLength();
}
