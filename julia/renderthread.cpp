#include <QtGui>

#include <math.h>

#include "renderthread.h"
#include <CudaBase.h>

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    restart = false;
    abort = false;
	aphid::CudaBase::SetDevice();
}

RenderThread::~RenderThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void RenderThread::render(QSize resultSize)
{
    QMutexLocker locker(&mutex);

    int w = resultSize.width();
    int h = resultSize.height();

	aphid::CudaRender::GetRoundedSize(w, h);
  
	this->resultSize = QSize(w, h);

    if (!isRunning()) {
        start(LowPriority);
    } else {
        restart = true;
        condition.wakeOne();
    }
}

void RenderThread::run()
{
    forever {
        mutex.lock();

        QSize resultSize = this->resultSize;

        mutex.unlock();
		
		if (restart)
			break;
		if (abort)
			return;
			
		m_render.setSize(resultSize.width(), resultSize.height() );
		m_render.render();
		
		QImage image(resultSize, QImage::Format_RGB32);
			
        const int tw = m_render.tileX();
        const int th = m_render.tileY();
        
        int i, j, k, l;
        for(j=0; j<th; ++j) {
            for(i=0; i<tw; ++i) {
                if (restart)
                    break;
                if (abort)
                    return;
                
				uint * tile = (uint *)m_render.tileHostColor(i, j);
                
				/*
				int r = rand()%256;
                int g = rand()%256;
                int b = rand()%256;
                for(k=0; k<16; ++k) {
                    for(l=0; l<16; ++l) {
                        tile[k * 16 +l] = qRgb(r, g, b);
                    }
                }
				*/
                
				uint *scanLine = reinterpret_cast<uint *>(image.scanLine(j * 16) );
				m_render.sendTileColor(&scanLine[i*16], resultSize.width(), i, j);
            }
        }
        
		if (!restart)
			emit renderedImage(image);

        mutex.lock();
		
        if (!restart)
            condition.wait(&mutex);
			
        restart = false;
        mutex.unlock();
    }
}

