#include <QtGui>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
	glWidget = new GLWidget;

	setCentralWidget(glWidget);
    setWindowTitle(tr("Narrow Phase"));
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
