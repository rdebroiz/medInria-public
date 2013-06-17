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
#include "../v3dViewPluginExport.h"

class v3dGraphicsScenePrivate;

class V3DVIEWPLUGIN_EXPORT v3dGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    v3dGraphicsScene(QObject *parent = 0);
    virtual ~v3dGraphicsScene();

private:
    v3dGraphicsScenePrivate *d;
};
