/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medVtkGraphicsView.h"
#include <QGraphicsWidget>

class medVtkGraphicsViewPrivate
{
public:
    QGraphicsWidget *graphicsWidget;
};

medVtkGraphicsView::medVtkGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    d(new medVtkGraphicsViewPrivate)
{
    d->graphicsWidget = NULL;

    setBackgroundBrush (QColor(0,0,0));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setRenderHint(QPainter::SmoothPixmapTransform, true);
}

medVtkGraphicsView::~medVtkGraphicsView()
{
    delete d;
}

void medVtkGraphicsView::setGraphicsWidget(QGraphicsWidget *graphicsWidget)
{
    d->graphicsWidget = graphicsWidget;
}

void medVtkGraphicsView::resizeEvent(QResizeEvent *event)
{
    if (!d->graphicsWidget)
        return;

    d->graphicsWidget->setGeometry(this->geometry());
    //graphicsWidget->resizeEvent(event);

    QGraphicsView::resizeEvent(event);
}
