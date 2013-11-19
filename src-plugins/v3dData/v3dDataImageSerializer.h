/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include "v3dDataPluginExport.h"

#include <dtkCore/dtkAbstractDataFactory.h>
#include <dtkCore/dtkAbstractDataSerializer.h>

class v3dDataImage;
class v3dDataImageSerializerPrivate;

class V3DDATAPLUGIN_EXPORT v3dDataImageSerializer: public dtkAbstractDataSerializer
{
    Q_OBJECT

public:
     v3dDataImageSerializer(void);
     v3dDataImageSerializer(const v3dDataImageSerializer& other);
    ~v3dDataImageSerializer(void);

public:
    static bool registered(void);

    QString description(void) const;
    QStringList handled(void) const;

    QByteArray *data(void);

public slots:
    QByteArray *serialize(void) {
        return NULL;
    }
    QByteArray *serialize(dtkAbstractData *data);

public slots:
    QByteArray *serialize(v3dDataImage *data);

private:
    DTK_DECLARE_PRIVATE(v3dDataImageSerializer);
};


dtkAbstractDataSerializer *createV3dDataImageSerializer(void);
