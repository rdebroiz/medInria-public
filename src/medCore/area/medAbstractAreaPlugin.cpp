/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractAreaPlugin.h>

void medAbstractAreaPluginFactory::record(const QString &key, creator func, unsigned int idx)
{
    m_sortedkeys.insert(idx, key);
    dtkCorePluginFactory<medAbstractArea>::record(key, func);
}

QStringList medAbstractAreaPluginFactory::keys() const
{
    return m_sortedkeys;
}
