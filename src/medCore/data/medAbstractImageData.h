/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <typeinfo>
#include <vector>

#include <medCoreExport.h>
#include <medAbstractData.h>

class MEDCORE_EXPORT medAbstractImageData: public medAbstractData
{
    Q_OBJECT

public:
    typedef std::vector < std::vector <double> > MatrixType;
    typedef std::vector <double> SizeType;
    enum ComponentType
    {
        UCHAR,
        CHAR,
        UINT,
        INT,
        FLOAT,
        DOUBLE
    };

    medAbstractImageData();
    virtual ~medAbstractImageData();

    virtual void* image() const {}
    virtual void setImage(void *image) {Q_UNUSED(image);}

    int dimension() const;
    ComponentType componentType() const;
    unsigned int numberOfComponent() const;
    MatrixType orientationMatrix() const;
    SizeType size() const;

    struct medAbstractImageDataMembers
    {
        ComponentType componentType;
        MatrixType orientationMatrix;
        unsigned int dimension;
        unsigned int numberOfComponent;
        SizeType size;
        void* image;
    };
    void setImageDataMembers(medAbstractImageDataMembers *m);

private:
    medAbstractImageDataMembers *members;
};

Q_DECLARE_METATYPE(medAbstractImageData)
Q_DECLARE_METATYPE(medAbstractImageData *)
