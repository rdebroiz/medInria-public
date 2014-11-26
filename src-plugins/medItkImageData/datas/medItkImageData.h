/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once


#include <medItkImageExport.h>
#include <medAbstractImageData.h>
#include <medAbstractDataFactory.h>

#include <itkImageBase.h>


class MEDITKIMAGEPLUGIN_EXPORT medItkImageData: public medAbstractImageData
{
    Q_OBJECT

    MED_DATA_INTERFACE("medItkImageData", "itk::image<TPix, DIM> in a shell...")

    static bool registered()
    {
        return medAbstractDataFactory::instance()->registerDataType<medItkImageData>();
    }

protected:
    itk::SmartPointer<itk::ImageBase<3> > imageData;

public:
    virtual void setImage(void* im)
    {
        imageData = dynamic_cast<itk::ImageBase<3> *>(static_cast<itk::Object*>(im));
        if(imageData.IsNull())
            qDebug() << "Cannot cast data to correct data type";
    }
    virtual void* image() const
    {
        return imageData;
    }
};
