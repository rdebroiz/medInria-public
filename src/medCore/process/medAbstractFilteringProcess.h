/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medCoreExport.h>

#include <medAbstractProcess.h>
#include <dtkCore/dtkAbstractProcess_p.h>


class medAbstractData;
class medAbstractFilteringProcessPrivate;

class MEDCORE_EXPORT medAbstractFilteringProcess : public medAbstractProcess
{
    Q_OBJECT

public:
    medAbstractFilteringProcess(medAbstractProcess *parent = NULL);
    virtual ~medAbstractFilteringProcess();

    virtual void setInputImage(medAbstractData *data) = 0;

private:
//    DTK_DECLARE_PRIVATE(medAbstractFilteringProcess);

};


//class MEDCORE_EXPORT medAbstractFilteringProcessPrivate : public dtkAbstractProcessPrivate
//{
//public:
//    medAbstractFilteringProcessPrivate(medAbstractFilteringProcess *q = 0) : dtkAbstractProcessPrivate(q) {}
//    medAbstractFilteringProcessPrivate(const medAbstractFilteringProcessPrivate& other) : dtkAbstractProcessPrivate(other) {}

//public:
//    virtual ~medAbstractFilteringProcessPrivate(void) {}

//};

//DTK_IMPLEMENT_PRIVATE(medAbstractFilteringProcess, medAbstractProcess)
