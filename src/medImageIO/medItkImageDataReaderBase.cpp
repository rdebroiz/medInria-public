/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medItkImageDataReaderBase.h>

#include <medAbstractImageData.h>
#include <medMetaDataKeys.h>

#include <medAbstractData.h>
#include <medAbstractDataFactory.h>
#include <dtkCore/dtkSmartPointer.h>

#include <itkImageFileReader.h>
#include <itkRGBPixel.h>
#include <itkVector.h>
#include <itkRGBAPixel.h>
#include <itkObjectFactoryBase.h>
#include <itkMetaDataObject.h>

template <class ComponentType, unsigned int Dim >
void
createImage(itk::ImageIOBase::Pointer imageIO, medAbstractImageData::medAbstractImageDataMembers *m, std::string path, medAbstractImageData *data)
{
    switch(imageIO->GetPixelType())
    {
        case itk::ImageIOBase::SCALAR:
        {
            m->numberOfComponent = 1;
            typedef itk::Image<ComponentType, Dim> ImageType;
            itk::SmartPointer<itk::Image<ComponentType, Dim> >im;
            typename itk::ImageFileReader<ImageType>::Pointer scalarReader = itk::ImageFileReader<ImageType>::New();
            scalarReader->SetImageIO(imageIO);
            scalarReader->SetFileName(path);
            scalarReader->SetUseStreaming(true);
            im = scalarReader->GetOutput();
            scalarReader->Update();
            m->image = im;
            for(int i = 0; i < Dim; ++i)
                m->size.push_back(im->GetLargestPossibleRegion().GetSize()[i]);

            qDebug() << m->size[0] << m->size[1] << m->size[2];
            data->setImageDataMembers(m);
            data->setImage(im);

            break;
        }
        case itk::ImageIOBase::VECTOR:
        {
            m->numberOfComponent = imageIO->GetNumberOfComponents();
            typedef itk::VectorImage<ComponentType, Dim> VectorImageType;
            typename VectorImageType::Pointer im;
            typename itk::ImageFileReader<VectorImageType>::Pointer vectorReader = itk::ImageFileReader<VectorImageType>::New();
            vectorReader->SetFileName(path);
            im = vectorReader->GetOutput();
            vectorReader->Update();
            m->image = im;
            for(int i = 0; i < Dim; ++i)
                m->size.push_back(im->GetLargestPossibleRegion().GetSize()[i]);
            data->setImageDataMembers(m);
            data->setImage(im);
            break;
        }
    }
}

template <class ComponentType >
void
retrieveNbDimensions(itk::ImageIOBase::Pointer imageIO, medAbstractImageData::medAbstractImageDataMembers *m, std::string path, medAbstractImageData *data)
{
    unsigned int nbDim = imageIO->GetNumberOfDimensions();
    switch(nbDim)
    {
    case 2:
    case 3:
        createImage<ComponentType, 3>(imageIO, m, path, data);
        m->dimension = 3;
        break;
    case 4:
        createImage<ComponentType, 4>(imageIO, m, path, data);
        m->dimension = 4;
        break;
    }
}


medItkImageDataReaderBase::medItkImageDataReaderBase() : dtkAbstractDataReader()
{
    this->io = 0;
}

medItkImageDataReaderBase::~medItkImageDataReaderBase()
{
}

bool medItkImageDataReaderBase::read(const QString& path)
{
    if(!this->readInformation(path))
        return false;

    medAbstractImageData *data = dynamic_cast<medAbstractImageData *>(medAbstractDataFactory::instance()->create("medItkImageData"));

    medAbstractImageData::medAbstractImageDataMembers *members = new medAbstractImageData::medAbstractImageDataMembers;
    switch(io->GetComponentType())
    {
        case itk::ImageIOBase::UCHAR:
        case itk::ImageIOBase::USHORT:
            members->componentType = medAbstractImageData::UCHAR;
            retrieveNbDimensions<unsigned char>(io, members, path.toStdString(), data);
            break;
        case itk::ImageIOBase::UINT:
        case itk::ImageIOBase::ULONG:
            members->componentType = medAbstractImageData::UINT;
            retrieveNbDimensions<unsigned int>(io, members, path.toStdString(), data);
            break;
        case itk::ImageIOBase::CHAR:
        case itk::ImageIOBase::SHORT:
            members->componentType = medAbstractImageData::CHAR;
            retrieveNbDimensions<char>(io, members, path.toStdString(), data);
            break;
        case itk::ImageIOBase::INT:
        case itk::ImageIOBase::LONG:
            members->componentType = medAbstractImageData::INT;
            retrieveNbDimensions<int>(io, members, path.toStdString(), data);
            break;
        case itk::ImageIOBase::FLOAT:
            members->componentType = medAbstractImageData::FLOAT;
            retrieveNbDimensions<float>(io, members, path.toStdString(), data);
            break;
        case itk::ImageIOBase::DOUBLE:
            members->componentType = medAbstractImageData::DOUBLE;
            retrieveNbDimensions<double>(io, members, path.toStdString(), data);
            break;
    }

    this->setData(data);

    return true;
}

bool medItkImageDataReaderBase::read (const QStringList& paths)
{
    if (paths.count() < 1)
        return false;
    return this->read(paths[0].toAscii().constData());
}

void medItkImageDataReaderBase::setProgress (int value)
{
    emit progressed (value);
}

bool medItkImageDataReaderBase::readInformation(const QString &path)
{
    if (this->io.IsNull())
        return false;

    this->io->SetFileName(path.toAscii().constData());
    try
    {
        this->io->ReadImageInformation();
    }
    catch (itk::ExceptionObject &e)
    {
        qDebug() << e.GetDescription();
        return false;
    }

    return true;
}

bool medItkImageDataReaderBase::readInformation(const QStringList &paths)
{
    if (!paths.count())
        return false;

    return this->readInformation ( paths[0].toAscii().constData() );
}


bool medItkImageDataReaderBase::canRead (const QString& path)
{
    if (this->io.IsNull())
        return false;
    if (!this->io->CanReadFile( path.toAscii().constData()))
        return false;
    else
        return true;
}
bool medItkImageDataReaderBase::canRead (const QStringList& paths)
{
    if (!paths.count())
    return false;
    return this->canRead ( paths[0].toAscii().constData() );
}
