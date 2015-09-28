/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractDatasource.h>
#include <medMoveCommandItem.h>

class medPacsDatasourcePrivate;
class medToolBox;

/**
 * @class Pacs connection datasource that comes with treeview,
 * searchpanel and source selector (DICOM-nodes)
 */
class medPacsDatasource : public medAbstractDatasource
{
    Q_OBJECT
public:

    medPacsDatasource(QWidget* parent = 0);
    ~medPacsDatasource();

    QWidget* mainViewWidget();

    QWidget* sourceSelectorWidget();

    QString tabName();

    QList<medToolBox*> getToolBoxes();

    QString description() const;

private slots:
    void onPacsMove( const QVector<medMoveCommandItem>& cmdList);

private:
    medPacsDatasourcePrivate* d;

};


