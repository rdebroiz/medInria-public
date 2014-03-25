/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractFibersData.h>

#include <v3dDataPluginExport.h>

class v3dDataFibersPrivate;

class V3DDATAPLUGIN_EXPORT v3dDataFibers : public medAbstractFibersData
{
    Q_OBJECT

public:
             v3dDataFibers();
    virtual ~v3dDataFibers();

    virtual QString description() const;
    virtual QString identifier() const;
    static bool registered();

public:
    void *data();

    void setData(void *data);

private:
    v3dDataFibersPrivate *d;
};

medAbstractData *createV3dDataFibers();


