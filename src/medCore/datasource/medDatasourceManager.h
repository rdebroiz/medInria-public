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
#include <QUuid>

#include <medDataIndex.h>

class medAbstractDatasource;
class medDatasourceManagerPrivate;
class medAbstractData;
class medDatabaseDatasource;


class medDatasourceManager : public QObject
{
    Q_OBJECT

public:
    static medDatasourceManager *instance();
    static void destroy();
    QList<medDatabaseDatasource*> datasources() const;
    medDatabaseDatasource* datasource(QString const& identifier) const;

signals:
    void openRequest(const medDataIndex&);

protected:
    medDatasourceManager();
    ~medDatasourceManager();

private:

    static medDatasourceManager *s_instance;
    medDatasourceManagerPrivate *d;
};
