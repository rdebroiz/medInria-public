/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractData.h>
#include <itkFiltersProcessBase.h>

#include <itkFiltersPluginExport.h>

class itkFiltersDilateProcessPrivate;
class medAbstractData;

class ITKFILTERSPLUGIN_EXPORT itkFiltersDilateProcess : public itkFiltersProcessBase
{
    Q_OBJECT
    Q_PROPERTY(double radius READ radius WRITE setRadius)
    
public:
    itkFiltersDilateProcess(itkFiltersDilateProcess * parent = 0);
    itkFiltersDilateProcess(const itkFiltersDilateProcess& other);
    virtual ~itkFiltersDilateProcess(void);

public:
    QString identifier(void) const;

    static bool registered ( void );
    
public slots:

    void setParameter ( double  data, int channel );
    int update ( void );

 protected:
    double radius(void) const;
    void setRadius(double  radius);

private:
    DTK_DECLARE_PRIVATE(itkFiltersDilateProcess)
};

dtkAbstractProcess * createitkFiltersDilateProcess(void);
