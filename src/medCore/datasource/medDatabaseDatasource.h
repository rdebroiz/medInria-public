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
#include <medDataIndex.h>

class medDatabaseDatasourcePrivate;

/**
* Not a classical data source per se, as it does not import data
* Still it fits to the design of data sources
*/
class medDatabaseDatasource : public medAbstractDatasource
{
    Q_OBJECT

public:
    medDatabaseDatasource(QWidget *parent = 0);
    ~medDatabaseDatasource();

    virtual QWidget* mainViewWidget();
    QWidget* compactViewWidget();
    virtual QWidget* sourceSelectorWidget();
    virtual QString tabName();
    virtual QList<medToolBox*> getToolBoxes();
    virtual QString description() const;

public slots:
    void warnOpeningFailed(const medDataIndex& index);
    void visualiseItem(const medDataIndex& index);
    void exportItem(const medDataIndex& index);
    void removeItem(const medDataIndex& index);
    void saveItem(const medDataIndex& index);
    void createNewPatiet(const medDataIndex& index);
    void createNewStudy(const medDataIndex& index);
    void editDatabaseItem(const medDataIndex& index);

signals:
    void openRequest(const medDataIndex&);
    void openingFaild(const medDataIndex&);


protected slots:
    void onFilter(const QString &text, int column);


private:
    medDatabaseDatasourcePrivate* d;

};
