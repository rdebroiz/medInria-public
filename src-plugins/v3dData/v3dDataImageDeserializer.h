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

#include <dtkCore/dtkAbstractDataDeserializer.h>

class v3dDataImage;
class v3dDataImageDeserializerPrivate;

class V3DDATAPLUGIN_EXPORT v3dDataImageDeserializer : public dtkAbstractDataDeserializer
{
    Q_OBJECT

public:
    v3dDataImageDeserializer(void);
    v3dDataImageDeserializer(const v3dDataImageDeserializer& other);
    ~v3dDataImageDeserializer(void);

    static bool registered(void);

    QString description(void) const ;
    QStringList handled(void) const ;

public:
    dtkAbstractData *data(void);
    v3dDataImage *image(void);

/* public: */
/*     void setData(dtkAbstractData *data); */

public slots:
    dtkAbstractData *deserialize(const QByteArray &binary);

private:
    DTK_DECLARE_PRIVATE(v3dDataImageDeserializer);
};

dtkAbstractDataDeserializer *createV3dDataImageDeserializer(void);

