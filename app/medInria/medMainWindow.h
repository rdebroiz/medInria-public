/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QMainWindow>

class medMainWindowPrivate;
class medMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    medMainWindow(QWidget *parent = 0);
   ~medMainWindow();

private:
    medMainWindowPrivate *d;

};
