/*=========================================================================
 medInria
 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.
=========================================================================*/

#include <medDatasourceManager.h>

#include <dtkCore>

#include <QList>
#include <QUuid>

#include <medAbstractDatasource.h>
#include <medDataManager.h>
#include <medMetaDataKeys.h>
#include <medStorage.h>
#include <medMessageController.h>
#include <medCoreLayer.h>


class medDatasourceManagerPrivate
{
public:
    QHash <QString, medAbstractDatasource*> dataSources;

};

medDatasourceManager::medDatasourceManager(): d(new medDatasourceManagerPrivate)
{
    for(QString const& key : medCoreLayer::datasource::pluginFactory().keys())
    {
        dtkDebug()<< "factory creates dataSource:" << dataSourceName;
        medAbstractDatasource *datasource = medCoreLayer::datasource::pluginFactory().create(key);
        d->dataSources.insert(key, datasource);
        connect(dataSource, &medAbstractDatasource::openRequest,
                this, &medDatasourceManager::openRequest);
    }
}


QList<medAbstractDatasource*> medDatasourceManager::datasources() const
{
    d->dataSources.values();
}

medAbstractDatasource* medDatasourceManager::datasource(const QString &identifier) const
{
    medAbstractDatasource* datasource = NULL;
    if(d->dataSources.contains(identifier))
        datasource = datasources[identifier];
    else
        dtkWarn << "attempt to retrieve non existing datasource: " << identifier;

    return datasource;
}

medDatasourceManager * medDatasourceManager::instance( void )
{
    if(!s_instance)
        s_instance = new medDatasourceManager;
    return s_instance;
}

medDatasourceManager::~medDatasourceManager( void )
{
    delete d;
    d = NULL;
}

void medDatasourceManager::destroy( void )
{
    if (s_instance)
    {
        delete s_instance;
        s_instance = 0;
    }
}

medDatasourceManager *medDatasourceManager::s_instance = NULL;
