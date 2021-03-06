/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractData.h>
#include <itkFiltersProcessBase.h>

#include <itkFiltersPluginExport.h>

class itkFiltersCloseProcessPrivate;
class medAbstractData;

class ITKFILTERSPLUGIN_EXPORT itkFiltersCloseProcess : public itkFiltersProcessBase
{
    Q_OBJECT
    
public:
    itkFiltersCloseProcess(itkFiltersCloseProcess * parent = 0);
    itkFiltersCloseProcess(const itkFiltersCloseProcess& other);
    virtual ~itkFiltersCloseProcess(void);

    static bool registered ( void );
    
public slots:

    void setParameter ( double  data, int channel );
    int update ( void );

private:
    DTK_DECLARE_PRIVATE(itkFiltersCloseProcess)
};

dtkAbstractProcess * createitkFiltersCloseProcess(void);
