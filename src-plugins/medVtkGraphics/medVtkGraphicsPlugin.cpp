/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medVtkGraphics.h"
#include "medVtkGraphicsPlugin.h"

#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
// medVtkGraphicsPluginPrivate
// /////////////////////////////////////////////////////////////////

class medVtkGraphicsPluginPrivate
{
public:
    // Class variables go here.
};

// /////////////////////////////////////////////////////////////////
// medVtkGraphicsPlugin
// /////////////////////////////////////////////////////////////////

medVtkGraphicsPlugin::medVtkGraphicsPlugin(QObject *parent) : dtkPlugin(parent), d(new medVtkGraphicsPluginPrivate)
{

}

medVtkGraphicsPlugin::~medVtkGraphicsPlugin()
{
    delete d;

    d = NULL;
}

bool medVtkGraphicsPlugin::initialize()
{
    if (!medVtkGraphics::registered())                     { dtkWarn() << "Unable to register medVtkGraphics type";                     }
    return true;
}

bool medVtkGraphicsPlugin::uninitialize()
{
    return true;
}

QString medVtkGraphicsPlugin::name() const
{
    return "medVtkGraphicsPlugin";
}

QString medVtkGraphicsPlugin::contact() const
{
    return "";
}

QStringList medVtkGraphicsPlugin::authors() const
{
    QStringList list;
    list << QString::fromUtf8("Pierre Fillard");
    return list;
}

QStringList medVtkGraphicsPlugin::contributors() const
{
    QStringList list;
    list <<  "Nicolas Toussaint"
          << "Stephan Schmitt"
          << "John Stark"
          << QString::fromUtf8("Benoît Bleuzé")
          << "Jaime Garcia"
          << "Fatih Arslan"
          << "Sergio Medina";
    return list;
}

QString medVtkGraphicsPlugin::version() const
{
    return MEDVTKGRAPHICSPLUGIN_VERSION;
}

QString medVtkGraphicsPlugin::description() const
{
    return tr("View Plugin\n<br/>"
              "Contains view and interactors based on vtkinria3d");
}

QStringList medVtkGraphicsPlugin::tags() const
{
    return QStringList() << "v3d" << "view";
}

QStringList medVtkGraphicsPlugin::types() const
{
    return QStringList() << medVtkGraphics::s_identifier();
}

Q_EXPORT_PLUGIN2(medVtkGraphicsPlugin, medVtkGraphicsPlugin)
