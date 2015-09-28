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
#include <QPushButton>

#include <medGuiLayer.h>
#include <medAbstractArea.h>
#include <medSettingsManager.h>

class medMainWindowPrivate
{
public:

};

medMainWindow::medMainWindow(QWidget *parent): QMainWindow(parent),
    d(new medMainWindowPrivate)
{


    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    this->setCentralWidget(mainWidget);
    mainWidget->setLayout(mainLayout);

    QStackedWidget* centralStack = new QStackedWidget;
    mainLayout->addWidget(centralStack);

    QWidget* statusBar = new QWidget;
    statusBar->setObjectName("medStatusBar");
    mainLayout->addWidget(statusBar);
    QHBoxLayout *statusBarLayout = new QHBoxLayout;
    statusBarLayout->setMargin(0);
    statusBar->setLayout(statusBarLayout);
    statusBarLayout->addSpacing(5);

    QSignalMapper *centralWidgetSignalMapper = new QSignalMapper(this);
    connect(centralWidgetSignalMapper, SIGNAL(mapped(QWidget*)),
            centralStack, SLOT(setCurrentWidget(QWidget*)));

    QStackedWidget* statusBarStack = new QStackedWidget;
    QSignalMapper *statusBarSignalMapper = new QSignalMapper(this);
    connect(statusBarSignalMapper, SIGNAL(mapped(QWidget*)),
            statusBarStack, SLOT(setCurrentWidget(QWidget*)));

    unsigned int areaIdx = 0;
    for(QString const& key : medGuiLayer::area::pluginFactory().keys())
    {

        medAbstractArea *area = medGuiLayer::area::pluginFactory().create(key);
        QWidget * centralWidget = area->centralWidget();
        QWidget * statusBarWidget = area->statusBarWidget();

        centralStack->addWidget(centralWidget);
        statusBarStack->addWidget(statusBarWidget);
        QPushButton *areaSwitcherButton = new QPushButton(area->icon(), area->title() +
                                                          " | " +
                                                          QString::number(areaIdx + 1));
        areaSwitcherButton->setShortcut(Qt::CTRL + Qt::Key_F1 + areaIdx);

        statusBarLayout->addWidget(areaSwitcherButton);

        connect(areaSwitcherButton, SIGNAL(clicked(bool)),
                centralWidgetSignalMapper, SLOT(map()));
        connect(areaSwitcherButton, SIGNAL(clicked(bool)),
                statusBarSignalMapper, SLOT(map()));
        centralWidgetSignalMapper->setMapping(areaSwitcherButton, centralWidget);
        statusBarSignalMapper->setMapping(areaSwitcherButton, statusBarWidget);

        ++areaIdx;
    }

    statusBarLayout->addStretch();
//    statusBarLayout->addWidget(statusBarStack, Qt::AlignRight);

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
