/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QWidget>
#include <QString>

#include <medCoreExport.h>

class MEDCORE_EXPORT medAbstractArea : public QObject
{
    Q_OBJECT

public:
    medAbstractArea(QObject* parent = NULL): QObject(parent) {}
    virtual ~medAbstractArea() {}

    virtual QString title() const = 0;
    virtual QString description() const = 0;

    virtual QIcon icon() const = 0;
    virtual QPixmap altTabPixmap() const =0;

    virtual QWidget* centralWidget() const = 0;
    virtual QWidget* statusBarWidget() const = 0;
};
