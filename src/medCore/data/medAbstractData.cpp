/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medAbstractData.h"

#include "medDataIndex.h"
#include "medAttachedData.h"

#include <dtkCore/dtkSmartPointer.h>

class medAbstractDataPrivate
{
public:
    medDataIndex index;
    QList< dtkSmartPointer<medAttachedData> > attachedData;
};

medAbstractData::medAbstractData( medAbstractData *parent )
    : dtkAbstractData(parent)
    , d(new medAbstractDataPrivate)
{
    qDebug() << "constructing medAbstractData: ";
}


medAbstractData::~medAbstractData( void )
{
    qDebug() << "deleting data with index " << d->index.asString();
    delete d;
    d = NULL;
}

void medAbstractData::setDataIndex( const medDataIndex& index )
{
    // copy ids
    d->index.setDataSourceId(index.dataSourceId());
    d->index.setPatientId(index.patientId());
    d->index.setStudyId(index.studyId());
    d->index.setSeriesId(index.seriesId());
    d->index.setImageId(index.imageId());
}

medDataIndex medAbstractData::dataIndex() const
{
    return d->index;
}

QList< medAttachedData * > medAbstractData::attachedData() const
{
    QList< medAttachedData * > ret;
#if QT_VERSION > 0x0406FF
    ret.reserve(d->attachedData.size());
#endif
    foreach( medAttachedData * data, d->attachedData ) {
        ret.push_back(data);
    }
    return ret;
}

void medAbstractData::clearAttachedData()
{
    foreach ( medAttachedData * data, d->attachedData )
        emit attachedDataRemoved( data );
    d->attachedData.clear();
}

void medAbstractData::addAttachedData( medAttachedData * data )
{
    if ( !d->attachedData.contains( data ) ) {
        d->attachedData.append( data );
        data->setParentData(this);
        emit attachedDataAdded( data );
    }
}

void medAbstractData::removeAttachedData( medAttachedData * data )
{
    int n = d->attachedData.count();
    d->attachedData.removeAll( data );
    if ( n > d->attachedData.count() ) {
        emit attachedDataRemoved( data );
        data->setParentData(NULL);
    }
}

void medAbstractData::invokeModified()
{
    emit dataModified(this);
}



void medAbstractData::update(void)
{
    DTK_DEFAULT_IMPLEMENTATION;
}

void *medAbstractData::output(void)
{
    DTK_DEFAULT_IMPLEMENTATION;

    return NULL;
}

void *medAbstractData::output(int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(channel);

    return NULL;
}

double medAbstractData::parameter(int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(channel);

    return -1;
}

void medAbstractData::setParameter(int parameter)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
}

void medAbstractData::setParameter(int parameter, int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
    DTK_UNUSED(channel);
}

void medAbstractData::setParameter(qlonglong parameter)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
}

void medAbstractData::setParameter(qlonglong parameter, int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
    DTK_UNUSED(channel);
}

void medAbstractData::setParameter(float parameter)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
}

void medAbstractData::setParameter(float parameter, int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
    DTK_UNUSED(channel);
}

void medAbstractData::setParameter(double parameter)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
}

void medAbstractData::setParameter(double parameter, int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
    DTK_UNUSED(channel);
}

void medAbstractData::setParameter(const QString& parameter)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
}

void medAbstractData::setParameter(const QString& parameter, int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
    DTK_UNUSED(channel);
}

void medAbstractData::setParameter(dtkAbstractData *parameter)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
}

void medAbstractData::setParameter(dtkAbstractData *parameter, int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
    DTK_UNUSED(channel);
}

void medAbstractData::setParameter(dtkVectorReal parameter)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
}

void medAbstractData::setParameter(dtkVectorReal parameter, int channel)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(parameter);
    DTK_UNUSED(channel);
}

QVariant medAbstractData::toVariant(dtkAbstractData *data)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(data);

    return qVariantFromValue(*data);
}

dtkAbstractData *medAbstractData::fromVariant(const QVariant& v)
{
    DTK_DEFAULT_IMPLEMENTATION;
    DTK_UNUSED(v);

    return NULL;
}
