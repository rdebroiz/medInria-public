/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#include "v3dDataImageSerializer.h"

#include "v3dDataImage.h"

#include "vtkDataWriter.h"
#include "vtkImageData.h"
#include "vtkDataObject.h"
#include "vtkGenericDataObjectWriter.h"
#include <vtksys/ios/sstream>

#include <dtkCore/dtkAbstractDataSerializer_p.h>

// /////////////////////////////////////////////////////////////////
// v3dDataImageSerializerPrivate interface
// /////////////////////////////////////////////////////////////////

class v3dDataImageSerializerPrivate: public dtkAbstractDataSerializerPrivate
{
public:
    v3dDataImageSerializerPrivate(v3dDataImageSerializer *q = 0) : dtkAbstractDataSerializerPrivate(q) {}
    v3dDataImageSerializerPrivate(const v3dDataImageSerializerPrivate& other) : dtkAbstractDataSerializerPrivate(other) {}

public:
    ~v3dDataImageSerializerPrivate(void) {}

public:
    QByteArray *array;
};

// /////////////////////////////////////////////////////////////////
// v3dDataImageSerializer protected constructors
// /////////////////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(v3dDataImageSerializer, dtkAbstractDataSerializer);

// /////////////////////////////////////////////////////////////////
// v3dDataImageSerializer implementation
// /////////////////////////////////////////////////////////////////

v3dDataImageSerializer::v3dDataImageSerializer(void) : dtkAbstractDataSerializer(*new v3dDataImageSerializerPrivate, 0)
{
    DTK_D(v3dDataImageSerializer);

    d->array = NULL;
}

v3dDataImageSerializer::v3dDataImageSerializer(const v3dDataImageSerializer& other) : dtkAbstractDataSerializer(*new v3dDataImageSerializerPrivate(*other.d_func()), other)
{
}

v3dDataImageSerializer::~v3dDataImageSerializer(void)
{

}

bool v3dDataImageSerializer::registered(void)
{
    return dtkAbstractDataFactory::instance()->registerDataSerializerType("v3dDataImageSerializer",QStringList("v3dDataImage"),createV3dDataImageSerializer);
}

//! Describes v3dDataImageSerializer's features.
/*!
 *
 */
QString v3dDataImageSerializer::description(void) const
{
    return "v3dDataImageSerializer serialize a v3dDataImage into a QByteArray ";
}

//! Return the list of handled data.
/*!
 *
 */
QStringList v3dDataImageSerializer::handled(void) const
{
    return QStringList() << "v3dDataImage";
}


QByteArray *v3dDataImageSerializer::data(void)
{
    DTK_D(v3dDataImageSerializer);
    return d->array;
}

QByteArray  *v3dDataImageSerializer::serialize( dtkAbstractData *data)
{

    if (v3dDataImage *mesh = dynamic_cast<v3dDataImage *>(data)) {
        return this->serialize(mesh);
    } else {
        dtkWarn() << "v3dDataImageSerializer expects v3dDataImage type as input";
        return NULL;
    }

}

QByteArray  *v3dDataImageSerializer::serialize( v3dDataImage *image)
{
    DTK_D(v3dDataImageSerializer);

//    QDataStream out(array,QIODevice::WriteOnly);

    vtkImageData *imageData = reinterpret_cast<vtkImageData *>(image->output());
    vtkDataWriter* writer = vtkGenericDataObjectWriter::New();
    // vtkDataObject* d = data->NewInstance();
    // d->ShallowCopy(data);
    writer->SetInput(imageData);
    // d->Delete();
    // We add the image extents to the header, since the writer doesn't preserve
    // the extents.
    int *extent = imageData->GetExtent();
    double* origin = imageData->GetOrigin();
    vtksys_ios::ostringstream stream;
    stream << "EXTENT " << extent[0] << " " <<
      extent[1] << " " <<
      extent[2] << " " <<
      extent[3] << " " <<
      extent[4] << " " <<
      extent[5];
    stream << " ORIGIN: " << origin[0] << " " << origin[1] << " " << origin[2];

    writer->SetHeader(stream.str().c_str());
    writer->SetFileTypeToBinary();
    writer->WriteToOutputStringOn();
    writer->Write();

    // char* buffer =NULL;
    // vtkIdType buffer_length = 0;

    // buffer_length = writer->GetOutputStringLength();
    // buffer = writer->RegisterAndGetOutputString();

    QByteArray *array = new QByteArray(writer->RegisterAndGetOutputString(), writer->GetOutputStringLength());

    d->array = array;
    dtkAbstractDataSerializer::setData(array);
    writer->Delete();

    return array;
}


// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractDataSerializer *createV3dDataImageSerializer(void)
{
    return new v3dDataImageSerializer;
}

