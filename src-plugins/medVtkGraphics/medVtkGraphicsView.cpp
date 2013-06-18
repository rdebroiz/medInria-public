/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medVtkGraphicsView.h"

#include <QGraphicsScene>
#include <QGLWidget>
#include <QGLFormat>
#include <QResizeEvent>

#include "QVTKWidget2.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "medVtkGraphicsScene.h"

class medVtkGraphicsViewPrivate
{
public:
    QGraphicsWidget *graphicsWidget;
    
    QGLContext *  context;
    QVTKWidget2 * viewport;
};

medVtkGraphicsView::medVtkGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    d(new medVtkGraphicsViewPrivate)
{
    d->context = new QGLContext(QGLFormat());
    d->viewport = new QVTKWidget2(d->context);
    
    d->viewport->GetRenderWindow()->SetSwapBuffers(0);  // don't let VTK swap buffers on us
    d->viewport->setAutoBufferSwap(true);
    
    this->setViewport(d->viewport);
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    this->setScene(new medVtkGraphicsScene(d->context, this));

    this->setBackgroundBrush (QColor(0,0,0));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    this->setAcceptDrops(false);
}

medVtkGraphicsView::~medVtkGraphicsView()
{
    delete d;
}

void medVtkGraphicsView::drawBackground(QPainter* p, const QRectF& r)
{
#if QT_VERSION >= 0x040600
    p->beginNativePainting();
#endif
    d->viewport->GetRenderWindow()->PushState();
    d->viewport->GetRenderWindow()->Render();
    d->viewport->GetRenderWindow()->PopState();
#if QT_VERSION >= 0x040600
    p->endNativePainting();
#endif
}


void medVtkGraphicsView::resizeEvent(QResizeEvent *event)
{
    // give the same size to the scene that his widget has
    if (scene())
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    
    QGraphicsView::resizeEvent(event);
    d->viewport->GetRenderWindow()->SetSize(event->size().width(), event->size().height());
}
