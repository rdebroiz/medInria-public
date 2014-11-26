/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractImageData.h>

#include <QtGui>



medAbstractImageData::medAbstractImageData() : medAbstractData()
{
    qDebug() << "constructing medAbstractImageData";
    members = 0;
}

medAbstractImageData::~medAbstractImageData()
{
    qDebug() << "deleting medAbstractImageData";
    delete members;
}

int medAbstractImageData::dimension() const
{
    members->dimension;
}

medAbstractImageData::ComponentType medAbstractImageData::componentType() const
{
    members->componentType;
}

unsigned int medAbstractImageData::numberOfComponent() const
{
    members->numberOfComponent;
}

medAbstractImageData::MatrixType medAbstractImageData::orientationMatrix() const
{
    members->orientationMatrix;
}

medAbstractImageData::SizeType medAbstractImageData::size() const
{
    members->size;
}

void medAbstractImageData::setImageDataMembers(medAbstractImageDataMembers *m)
{
    members = m;
    this->setImage(m->image);
}
