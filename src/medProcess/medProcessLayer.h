/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractSubtractImageProcessPlugin.h>
#include <medAbstractGcStremMsLesionsSegmentationProcessPlugin.h>

namespace medProcessLayer
{
    namespace pluginManager
    {
        void initialize(const QString& path = QString());
    }

    namespace subtractImage
    {
        medAbstractSubtractImageProcessPluginManager& pluginManager();
        medAbstractSubtractImageProcessPluginFactory& pluginFactory();
        void initialize(const QString& path);

    }

    namespace gcStremMsLesionsSegmentation
    {
        medAbstractGcStremMsLesionsSegmentationProcessPluginManager &pluginManager();
        medAbstractGcStremMsLesionsSegmentationProcessPluginFactory &pluginFactory();
        void initialize(const QString& path);
    }
}
