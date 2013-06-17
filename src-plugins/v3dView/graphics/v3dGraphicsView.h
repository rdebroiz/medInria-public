/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once


#include <QGraphicsView>
#include "../v3dViewPluginExport.h"

class v3dGraphicsViewPrivate;

class V3DVIEWPLUGIN_EXPORT v3dGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    v3dGraphicsView(QWidget *parent = 0);
    virtual ~v3dGraphicsView();

private:
    v3dGraphicsViewPrivate *d;
};
