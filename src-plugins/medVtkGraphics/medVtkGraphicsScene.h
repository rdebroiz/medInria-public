/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once


#include <QGraphicsScene>
#include "medVtkGraphicsPluginExport.h"

#include <QtOpenGL/QGLContext>

class medVtkGraphicsScenePrivate;

class MEDVTKGRAPHICSPLUGIN_EXPORT medVtkGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    medVtkGraphicsScene(QGLContext* ctx, QObject *parent = 0);
    virtual ~medVtkGraphicsScene();

private:
    medVtkGraphicsScenePrivate *d;
};
