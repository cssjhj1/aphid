#include <QtGui>
#include <QtOpenGL>
#include <BaseCamera.h>
#include <PerspectiveView.h>
#include "widget.h"
#include <GeoDrawer.h>
#include <ogl/DrawGrid.h>
#include <ttg/TetrahedronGrid.h>
#include <ttg/TetraGridTriangulation.h>
#include <ogl/DrawGraph.h>
#include <geom/PrimInd.h>
#include <sdb/LodGrid.h>
#include <sdb/GridClosestToPoint.h>

using namespace aphid;

GLWidget::GLWidget(QWidget *parent) : Base3DView(parent)
{
	perspCamera()->setFarClipPlane(20000.f);
	perspCamera()->setNearClipPlane(1.f);
	orthoCamera()->setFarClipPlane(20000.f);
	orthoCamera()->setNearClipPlane(1.f);
	usePerspCamera();
	
}

GLWidget::~GLWidget()
{}

static const float stetvs[5][3] = {
{ -13.f, -3.f, -9.f}, 
{ 0.f, 17.f, -5.f},
{ 16.f, 2.f, -10.f}, 
{ 4.f, 0.f, 12.f},
{ -9.f, 9.f, -24.f}
};

void GLWidget::clientInit()
{
	cvx::Tetrahedron tetra[2];
	tetra[0].set(Vector3F(stetvs[0]), 
				Vector3F(stetvs[1]),
				Vector3F(stetvs[2]), 
				Vector3F(stetvs[3]) );
				
	cvx::Tetrahedron tetra1;
	tetra[1].set(Vector3F(stetvs[0]), 
				Vector3F(stetvs[2]),
				Vector3F(stetvs[1]), 
				Vector3F(stetvs[4]) );
		
    TetrahedronGridUtil<5 > tu4;
	
	for(int i=0;i<2;++i) {
		m_grd[i] = new GridT(tetra[i], 0);
		
		m_mesher[i] = new MesherT;
		m_mesher[i]->setGrid(m_grd[i]);
	}
    
    m_fieldDrawer = new FieldDrawerT;
    m_fieldDrawer->initGlsl();
    
static const float scCorners[6][3] = {
{4, 7, 0},
{2, 17, -9},
{-2, 6, -1},
{9, 6, -13},
{6, 12, -11},
{4, -5, 0}
};

    cvx::Triangle * ta = new cvx::Triangle;
	ta->set(Vector3F(scCorners[2]), Vector3F(scCorners[0]), Vector3F(scCorners[1]) );
	m_ground.push_back(ta);
	cvx::Triangle * tb = new cvx::Triangle;
	tb->set(Vector3F(scCorners[1]), Vector3F(scCorners[0]), Vector3F(scCorners[4]) );
	m_ground.push_back(tb);
    cvx::Triangle * tc = new cvx::Triangle;
	tc->set(Vector3F(scCorners[4]), Vector3F(scCorners[0]), Vector3F(scCorners[3]) );
	m_ground.push_back(tc);
    cvx::Triangle * td = new cvx::Triangle;
	td->set(Vector3F(scCorners[2]), Vector3F(scCorners[1]), Vector3F(-10, 5, -13) );
	m_ground.push_back(td);
	cvx::Triangle * te = new cvx::Triangle;
	te->set(Vector3F(scCorners[3]), Vector3F(scCorners[0]), Vector3F(scCorners[5]) );
	m_ground.push_back(te);
    cvx::Triangle * tf = new cvx::Triangle;
	tf->set(Vector3F(scCorners[5]), Vector3F(scCorners[0]), Vector3F(scCorners[2]) );
	m_ground.push_back(tf);
    
	m_sels.insert(0);
	m_sels.insert(1);
    m_sels.insert(2);
    m_sels.insert(3);
    m_sels.insert(4);
    m_sels.insert(5);
    
    TIntersect fintersect(&m_sels, &m_ground);
	
	BoundingBox rootBx;
	rootBx.expandBy(ta->calculateBBox() );
	rootBx.expandBy(tb->calculateBBox() );
	rootBx.expandBy(tc->calculateBBox() );
	rootBx.expandBy(td->calculateBBox() );
	rootBx.expandBy(te->calculateBBox() );
	rootBx.expandBy(tf->calculateBBox() );
	
	float sz0 = rootBx.getLongestDistance();
	
	m_lodg = new LodGridTyp;
	m_lodg->fillBox(rootBx, sz0);
	
	sdb::AdaptiveGridDivideProfle subdprof;
	subdprof.setLevels(0, 3);
	
	m_lodg->subdivideToLevel<TIntersect>(fintersect, subdprof);
	m_lodg->insertNodeAtLevel<TIntersect, 3 >(3, fintersect);
	
	m_selGrd = new SelGridTyp(m_lodg);
	m_selGrd->setMaxSelectLevel(3);
    
    Vector3F agp, agn;
    fintersect.getAggregatedPositionNormal(agp, agn);
        
	CalcDistanceProfile prof;
	prof.referencePoint = agp;
	prof.direction = agn;
	prof.offset = 0.f;
	prof.snapDistance = .1f;
	
	for(int i=0;i<2;++i) {
	
		m_mesher[i]->field()->calculateDistance<TIntersect>(m_grd[i], &fintersect, prof);
    //m_mesher[i]->field()->calculateDistance<SelGridTyp>(m_grd, m_selGrd, prof);
    
		m_mesher[i]->triangulate();
    
		m_frontMesh[i] = new ATriangleMesh;
		m_mesher[i]->dumpFrontTriangleMesh(m_frontMesh[i]);
		m_frontMesh[i]->calculateVertexNormals();
	}
    
	const BoundingBox bxc = te->calculateBBox();
	m_selGrd->select(bxc );
	
}

void GLWidget::clientDraw()
{
	//updatePerspectiveView();
	//getDrawer()->frustum(perspectiveView()->frustum() );
	
	getDrawer()->m_markerProfile.apply();
	getDrawer()->setColor(.5f, .125f, .5f);
	drawGround();
    
    getDrawer()->setColor(.025f, .25f, .125f);
	//drawSolidGrid();
    drawField();
    drawCellCut();
    
    drawTriangulation();
/*
    glEnable(GL_CULL_FACE);
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
    
    cvx::Tetrahedron tetra;
	tetra.set(Vector3F(stetvs[0]), 
				Vector3F(stetvs[1]),
				Vector3F(stetvs[2]), 
				Vector3F(stetvs[3]) );
    drawASolidTetrahedron(tetra);            
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
*/
}

void GLWidget::drawCellCut()
{
    TIntersect fintersect(&m_sels, &m_ground);
    
    glBegin(GL_LINES);
    cvx::Tetrahedron atet;
	
	for(int j=0;j<2;++j) {
		GridT * jg = m_grd[j];
		const int n = jg->numCells();
		for(int i=0;i<n;++i) {
			jg->getCell(atet, i);
        
        if(!fintersect.tetrahedronIntersect(atet) ) {
            continue;
        }
        
        Vector3F tcen = atet.getCenter();
        fintersect.closestToPoint(tcen);
        
        Vector3F pop = fintersect.closestToPointPoint();
        
        glColor3f(.01f, .3f, .2f);
        glVertex3fv((const float *)&tcen );
        glVertex3fv((const float *)&pop );
        glColor3f(.01f, .5f, .4f);
        glVertex3fv((const float *)&pop );
        Vector3F pnm = pop + fintersect.closestToPointNormal();
        glVertex3fv((const float *)&pnm );
    }
	}
    glEnd();
    
}

void GLWidget::drawGround()
{
    glBegin(GL_LINES);
    m_sels.begin();
    while(!m_sels.end() ) {
        const int & k = m_sels.key();
        const cvx::Triangle * t = m_ground[k];
        
        glVertex3fv((const float *)&t->X(0) );
        glVertex3fv((const float *)&t->X(1) );
        glVertex3fv((const float *)&t->X(1) );
        glVertex3fv((const float *)&t->X(2) );
        glVertex3fv((const float *)&t->X(2) );
        glVertex3fv((const float *)&t->X(0) );
        
        m_sels.next();
    }
    glEnd();
#if 0
	DrawGrid<LodGridTyp> drg(m_lodg);
	drg.drawLevelCells(3);

	glColor3f(1,1,0);
	const int nc = m_selGrd->numActiveCells();
	for(int i=0;i<nc;++i) {
		const sdb::Coord4 & k = m_selGrd->activeCellCoord(i);
		drg.drawCell(k);
	}
#endif
}

void GLWidget::drawGridEdges()
{
    TetraGridEdgeMap<GridT > & edges = m_mesher[0]->gridEdges();
    
    glBegin(GL_LINES);
    edges.begin();
    while(!edges.end() ) {
        const sdb::Coord2 & k = edges.key();
        
        glVertex3fv((const float *)&m_grd[0]->pos(k.x) );
        glVertex3fv((const float *)&m_grd[0]->pos(k.y) );
        
        edges.next();
    }
    glEnd();

}

void GLWidget::drawField()
{
	for(int i=0;i<2;++i) {
		m_fieldDrawer->drawEdge(m_mesher[i]->field() );
		m_fieldDrawer->drawNode(m_mesher[i]->field() );
	}
}

void GLWidget::drawSolidGrid()
{
    getDrawer()->m_surfaceProfile.apply();
	
        glEnable(GL_CULL_FACE);
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	cvx::Tetrahedron atet;
    const int n = m_grd[0]->numCells();
    for(int i=0;i<n;++i) {
        m_grd[0]->getCell(atet, i);
        drawAShrinkSolidTetrahedron(atet, .67f);
    }
    
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLWidget::drawWiredGrid()
{
    glEnableClientState(GL_VERTEX_ARRAY);
	cvx::Tetrahedron atet;
    const int n = m_grd[0]->numCells();
    for(int i=0;i<n;++i) {
        m_grd[0]->getCell(atet, i);
        drawAWireTetrahedron(atet);
    }
    
    glDisableClientState(GL_VERTEX_ARRAY);

}

void GLWidget::drawTriangulation()
{
    getDrawer()->m_surfaceProfile.apply();
     getDrawer()->setColor(1,.7,0);
	 
	 for(int i=0;i<2;++i) {
		ATriangleMesh * fm = m_frontMesh[i];
		const unsigned nind = fm->numIndices();
		const unsigned * inds = fm->indices();
		const Vector3F * pos = fm->points();
		const Vector3F * nms = fm->vertexNormals();
    
    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
    glNormalPointer(GL_FLOAT, 0, (GLfloat*)nms );
	glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)pos );
    glDrawElements(GL_TRIANGLES, nind, GL_UNSIGNED_INT, inds);
	
	}
    
    glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void GLWidget::clientSelect(Vector3F & origin, Vector3F & ray, Vector3F & hit)
{
}
//! [9]

void GLWidget::clientDeselect()
{
}

//! [10]
void GLWidget::clientMouseInput(Vector3F & stir)
{
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key()) {
		case Qt::Key_M:
			//m_scene->progressForward();
			break;
		case Qt::Key_N:
			//m_scene->progressBackward();
			break;
		default:
			break;
	}
	Base3DView::keyPressEvent(e);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
	Base3DView::keyReleaseEvent(event);
}
	