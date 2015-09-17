/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medMainWindow.h>

#include <QStackedWidget>
#include <QStatusBar>

class medMainWindowPrivate
{
public:
    QStackedWidget *centralStack;

    QStatusBar *statusBar;
    QStackedWidget *statusBarStack;
};

medMainWindow::medMainWindow(QWidget *parent): QMainWindow(parent),
    d(new medMainWindowPrivate)
{
    d->centralStack = new QStackedWidget;
    this->setCentralWidget(d->centralStack);

    d->statusBar = new QStatusBar;
    this->setStatusBar(d->statusBar);

}

medMainWindow::~medMainWindow()
{
    delete d;
}
