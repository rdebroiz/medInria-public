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
#include <QMenu>
#include <QHBoxLayout>
#include <QSpacerItem>

#include <medGuiLayer.h>
#include <medAbstractArea.h>
#include <medSettingsManager.h>

class medMainWindowPrivate
{
public:
    QStackedWidget *centralStack;

    QStatusBar *statusBar;
    QStackedWidget *statusBarStack;

    QMenu *swichAraeMenu;
};

medMainWindow::medMainWindow(QWidget *parent): QMainWindow(parent),
    d(new medMainWindowPrivate)
{
    d->centralStack = new QStackedWidget;
    this->setCentralWidget(d->centralStack);

    d->statusBar = new QStatusBar;
    this->setStatusBar(d->statusBar);
    QHBoxLayout *statusBarLayout = new QHBoxLayout;
    QWidget *permanentStatusBarWidget = new QWidget;
    statusBarLayout->setMargin(0);
    permanentStatusBarWidget->setLayout(statusBarLayout);
    d->statusBar->addPermanentWidget(permanentStatusBarWidget);

    d->swichAraeMenu = new QMenu("Switch area");
    statusBarLayout->addWidget(d->swichAraeMenu);
//    QSpacerItem statusBarSpacer()

    foreach(QString key, medGuiLayer::area::pluginFactory().keys())
    {
        medAbstractArea *area = medGuiLayer::area::pluginFactory().create(key);
        d->centralStack->addWidget(area);
        d->swichAraeMenu->addAction(area->title());
//        d->areas.insert(key, area);
    }

    this->_restoreSettings();
}

medMainWindow::~medMainWindow()
{
    delete d;
}

void medMainWindow::closeEvent(QCloseEvent *event)
{
    this->_saveSettings();
    QMainWindow::closeEvent(event);
}

void medMainWindow::_restoreSettings()
{
    medSettingsManager * mnger = medSettingsManager::instance();

    this->restoreState(mnger->value("medMainWindow", "state").toByteArray());
    this->restoreGeometry(mnger->value("medMainWindow", "geometry").toByteArray());
}

void medMainWindow::_saveSettings()
{
    medSettingsManager * mnger = medSettingsManager::instance();
    mnger->setValue("medMainWindow", "state", this->saveState());
    mnger->setValue("medMainWindow", "geometry", this->saveGeometry());
}
