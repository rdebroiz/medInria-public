/*=========================================================================
 medInria
 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.
=========================================================================*/

#pragma once

#include <QObject>

#include <dtkCore>

#include <medDataIndex.h>
#include <medCoreExport.h>

class medToolBox;
class medDataIndex;
class medAbstractData;

/**
 * @class medAbstractDatasource
 * @brief Abstract base class for dynamic data sources, e.g. plugins that act as database clients
 * This class defines access methods to the following widgets: a mainViewWidget,
 * a source selection widget and several ToolBoxes.
 * All dynamic data source implementation should derive from this class.
 **/
class MEDCORE_EXPORT medAbstractDatasource : public QObject
{
    Q_OBJECT

public:
    medAbstractDatasource(QObject *parent = 0): QOBject(parent) {}
    virtual ~medAbstractDatasource();

    /**
    * Returns the main view widget
    * This widget is used to navigate within the data, e.g. a qTreeWidget
    */
    virtual QWidget *mainViewWidget() = 0;
    /**
    * Returns the source selector widget
    * A widget that let's the user choose between different data locations
    */
    virtual QWidget *sourceSelectorWidget() = 0;

    /** Returns the tab name for the plugin using the data source*/
    virtual QString tabName() = 0;

    /** Returns all ToolBoxes owned by the source data plugin*/
    virtual QList<medToolBox*> getToolBoxes() = 0;

    /** Returns a short description of the data source */
    virtual QString description() const = 0;

signals:
    void openRequest(const medDataIndex &index);
};

DTK_DECLARE_OBJECT        (medAbstractDatasource*)
DTK_DECLARE_PLUGIN        (medAbstractDatasource, MEDCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(medAbstractDatasource, MEDCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(medAbstractDatasource, MEDCORE_EXPORT)
