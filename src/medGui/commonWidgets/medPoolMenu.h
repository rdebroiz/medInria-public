/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medGuiExport.h>

#include <QMenu>

class medPoolMenuPrivate;
class MEDGUI_EXPORT medPoolMenu : public QMenu
{
    Q_OBJECT

public:
    medPoolMenu(QWidget *parent = NULL);
    virtual ~medPoolMenu();

private:
    medPoolMenuPrivate *d;
};
