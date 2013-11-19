/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#include "v3dDataImageDeserializer.h"
#include "v3dDataImage.h"

#include <vtkCharArray.h>
#include <vtkDataReader.h>
#include <vtkImageData.h>
#include <vtkGenericDataObjectReader.h>

#include <dtkCore/dtkAbstractDataDeserializer_p.h>
#include <dtkCore/dtkAbstractDataFactory.h>

class v3dDataImageDeserializerPrivate: public dtkAbstractDataDeserializerPrivate
{
public:
    v3dDataImageDeserializerPrivate(v3dDataImageDeserializer *q = 0) : dtkAbstractDataDeserializerPrivate(q) {}
    v3dDataImageDeserializerPrivate(const v3dDataImageDeserializerPrivate& other) : dtkAbstractDataDeserializerPrivate(other),
                                                                                    image(other.image) {}

public:
    ~v3dDataImageDeserializerPrivate(void) {}

public:
    v3dDataImage *image;
};

// /////////////////////////////////////////////////////////////////
// v3dDataImageDeserializer protected constructors
// /////////////////////////////////////////////////////////////////

DTK_IMPLEMENT_PRIVATE(v3dDataImageDeserializer, dtkAbstractDataDeserializer);

// /////////////////////////////////////////////////////////////////
// v3dDataImageDeserializer implementation
// /////////////////////////////////////////////////////////////////

v3dDataImageDeserializer::v3dDataImageDeserializer(void) : dtkAbstractDataDeserializer(*new v3dDataImageDeserializerPrivate(this), 0)
{
    DTK_D(v3dDataImageDeserializer);

    d->image = NULL;
}

v3dDataImageDeserializer::v3dDataImageDeserializer(const v3dDataImageDeserializer& other) : dtkAbstractDataDeserializer(*new v3dDataImageDeserializerPrivate(*other.d_func()), other)
{

}

v3dDataImageDeserializer::~v3dDataImageDeserializer(void)
{
}


bool v3dDataImageDeserializer::registered(void)
{
    return dtkAbstractDataFactory::instance()->registerDataDeserializerType("v3dDataImageDeserializer",QStringList("v3dDataImage"),createV3dDataImageDeserializer);
}

//! Describes v3dDataImageDeserializer's features.
/*!
 *
 */
QString v3dDataImageDeserializer::description(void) const
{
    return "v3dDataImageDeserializer converts a QByteArray serialized mesh into a v3dDataImage object";
}
//! Return the list of handled data.
/*!
 *
 */
QStringList v3dDataImageDeserializer::handled(void) const
{
    return QStringList() << "v3dDataImage";
}

//! Returns num3sis mesh as dtkAbstractData.
/*!
 *
 */
dtkAbstractData *v3dDataImageDeserializer::data(void)
{
    DTK_D(v3dDataImageDeserializer);

    return d->image;
}

v3dDataImage *v3dDataImageDeserializer::image(void)
{
    DTK_D(v3dDataImageDeserializer);

    return d->image;
}

dtkAbstractData *v3dDataImageDeserializer::deserialize(const QByteArray &array)
{
    // static bool once = false;

    DTK_D(v3dDataImageDeserializer);

    d->image = reinterpret_cast<v3dDataImage *>(d->data);

    // Start deserialization

//    QDataStream input(array);

    const char* bufferArray      = array.data();
    vtkIdType bufferLength = array.count();

    char* realBuffer = 0;

    // Setup a reader.
    vtkDataReader *reader = vtkGenericDataObjectReader::New();
    reader->ReadFromInputStringOn();

    vtkCharArray* mystring = vtkCharArray::New();
    mystring->SetArray(const_cast<char *>(bufferArray), bufferLength, 1);
    reader->SetInputArray(mystring);
//    reader->Modified();
    reader->Update();

    int extent[6]   = {0, 0, 0, 0, 0, 0};
    float origin[3] = {0, 0, 0};
    sscanf(reader->GetHeader(),
           "EXTENT %d %d %d %d %d %d ORIGIN %f %f %f", &extent[0], &extent[1],
           &extent[2], &extent[3], &extent[4], &extent[5],
           &origin[0], &origin[1], &origin[2]);
    vtkImageData * image = vtkImageData::SafeDownCast(
        reader->GetOutputDataObject(0)->NewInstance());
    image->ShallowCopy(reader->GetOutputDataObject(0));
    image->SetOrigin(origin[0], origin[1], origin[2]);
    image->SetExtent(extent);

    mystring->Delete();
    reader->Delete();
    delete [] realBuffer;

    d->image->setData(image);

    return d->image ;
}


// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractDataDeserializer *createV3dDataImageDeserializer(void)
{
    return new v3dDataImageDeserializer;
}
