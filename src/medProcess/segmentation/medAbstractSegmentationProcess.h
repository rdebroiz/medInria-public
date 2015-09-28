/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractProcess.h>

class medAbstractSegmentationProcess : public medAbstractProcess
{
public:
    medAbstractSegmentationProcess(QObject *parent = NULL): medAbstractProcess(parent) {}
    virtual ~medAbstractSegmentationProcess() {}
};

