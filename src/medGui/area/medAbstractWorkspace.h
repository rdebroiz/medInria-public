/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractArea.h>

/**
 * @brief A Workspace holds medToolBoxes, medViewContainers and their relations.
 *
 *The main role of a workspace is to provide a coherent set of toolboxes and containers that interact with each other.
 *
 * A workspace is usually instantiated by a factory.
 * It owns several medViewContainers in a medTabbedViewContainers.
 * It also owns toolboxes, but does not place them, the medWorkspaceArea does it when
 * medWorkspaceArea::setupWorkspace is called.
 *
*/

struct QUuid;
class QListWidgetItem;
class QAction;
class medToolBox;
class medTabbedViewContainers;
class medDataIndex;
class medViewContainer;
class medAbstractParameterGroup;
class medViewParameterGroup;
class medLayerParameterGroup;

class medAbstractWorkspacePrivate;
class medAbstractWorkspace: public medAbstractArea
{
    Q_OBJECT

    medAbstractWorkspace(QWidget *parent = NULL);
    virtual ~medAbstractWorkspace();

    QList <medToolBox*> toolBoxes() const;
    medToolBox* selectionToolBox() const;
    void setDatabaseVisibility(bool);
    bool isDatabaseVisible() const;
    void setToolBoxesVisibility(bool);
    bool areToolBoxesVisible() const;
    medTabbedViewContainers * tabbedViewContainers() const;

    void setUserLayerPoolable(bool poolable);
    void setUserViewPoolable(bool poolable);
    void setUserLayerClosable(bool Closable);
    bool isUserLayerPoolable() const;
    bool isUserViewPoolable() const;
    bool isUserLayerClosable() const;

public slots:
    virtual void clear();
    virtual void addNewTab();
    void updateNavigatorsToolBox();
    void updateMouseInteractionToolBox();
    void updateLayersToolBox();
    void updateInteractorsToolBox();
    void clearWorkspaceToolBoxes();
    void addToolBox(medToolBox *toolbox);
    void removeToolBox(medToolBox *toolbox);

    virtual void open(const medDataIndex& index);

protected slots:
    void changeCurrentLayer(int row);
    void removeLayer();

    void addViewGroup(QString);
    void addLayerGroup(QString);
    void addViewGroup(medViewParameterGroup*);
    void addLayerGroup(medLayerParameterGroup*);
    void setViewGroups(QList<medViewParameterGroup*>);
    void setLayerGroups(QList<medLayerParameterGroup*>);

private slots:
    void buildTemporaryPool();

    void addViewstoGroup(QString);
    void removeViewsFromGroup(QString);

    void addLayerstoGroup(QString);
    void removeLayersFromGroup(QString);

    void removeViewGroup(QString);
    void removeLayerGroup(QString);

    void changeViewGroupColor(QString group, QColor color);
    void changeLayerGroupColor(QString group, QColor color);

private:
    QWidget* buildViewLinkMenu();
    QWidget* buildLayerLinkMenu(QList<QListWidgetItem*>);

private:
    medAbstractWorkspacePrivate *d;
};
