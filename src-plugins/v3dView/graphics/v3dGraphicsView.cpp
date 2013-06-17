/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "v3dGraphicsView.h"

v3dGraphicsView::v3dGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
  setBackgroundBrush (QColor(0,0,0));
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  setRenderHint(QPainter::SmoothPixmapTransform, true);
}

v3dGraphicsView::~v3dGraphicsView()
{

}

