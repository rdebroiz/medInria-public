/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkCore/dtkPlugin.h>

#include "medVtkGraphicsPluginExport.h"

class MEDVTKGRAPHICSPLUGIN_EXPORT medVtkGraphicsPluginPrivate;

class MEDVTKGRAPHICSPLUGIN_EXPORT medVtkGraphicsPlugin : public dtkPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkPlugin)

public:
     medVtkGraphicsPlugin(QObject *parent = 0);
    ~medVtkGraphicsPlugin();

    virtual bool initialize();
    virtual bool uninitialize();

    virtual QString name() const;
    virtual QString description() const;
    virtual QString version() const;

    virtual QStringList authors() const;
    virtual QString contact() const;
    virtual QStringList contributors() const;


    virtual QStringList tags() const;
    virtual QStringList types() const;

private:
     medVtkGraphicsPluginPrivate *d;
};


