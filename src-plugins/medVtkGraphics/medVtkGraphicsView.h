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
#include "medVtkGraphicsPluginExport.h"

class medVtkGraphicsViewPrivate;

class MEDVTKGRAPHICSPLUGIN_EXPORT medVtkGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    medVtkGraphicsView(QWidget *parent = 0);
    virtual ~medVtkGraphicsView();

private:
    medVtkGraphicsViewPrivate *d;
};
