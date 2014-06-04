/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medPoolMenu.h>

#include <QtGui>

class medPoolMenuPrivate
{
    public:

};

medPoolMenu::medPoolMenu(QWidget *parent): QMenu(parent),
    d(new medPoolMenuPrivate)
{

}

medPoolMenu::~medPoolMenu()
{
    delete d;
}
