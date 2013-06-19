/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medVtkGraphicsScene.h"

#include <QtGui>

class medVtkGraphicsScenePrivate
{
public:
    QGLContext* context;
};

medVtkGraphicsScene::medVtkGraphicsScene(QGLContext* ctx, QObject *parent) 
    : QGraphicsScene(parent), d(new medVtkGraphicsScenePrivate)
{
    d->context = ctx;
}

medVtkGraphicsScene::~medVtkGraphicsScene()
{
    delete d;
}

