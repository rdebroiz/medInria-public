/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medBrowserArea.h>

#include <medBrowserSourceSelectorToolBox.h>

#include <QtGui>
#include <QtWidgets>

#include <dtkCoreSupport/dtkGlobal.h>
#include <dtkGuiSupport/dtkFinder.h>

#include <medMessageController.h>
#include <medJobManager.h>
#include <medDataManager.h>

#include <medStorage.h>

#include <medDatabaseController.h>
#include <medDatabaseNonPersistentController.h>
#include <medDatabaseExporter.h>
#include <medDatabaseImporter.h>

#include <medProgressionStack.h>
#include <medToolBox.h>
#include <medToolBoxFactory.h>
#include <medToolBoxContainer.h>
#include <medPacsMover.h>
#include <medPacsWidget.h>
#include <medCompositeDataSetImporterSelectorToolBox.h>
#include <medAbstractDatasource.h>
#include <medDatasourceManager.h>

class medBrowserAreaCentralWidgetPrivate
{
public:

    medToolBoxContainer *toolboxContainer;
    medBrowserSourceSelectorToolBox *sourceSelectorToolBox;
    QList<medAbstractDatasource *> dataSources;
    QStackedWidget *stack;
};

medBrowserAreaCentralWidget::medBrowserAreaCentralWidget(QWidget *parent): QWidget(parent),
    d(new medBrowserAreaCentralWidgetPrivate)
{
    d->stack = new QStackedWidget;

    // Source toolbox
    d->sourceSelectorToolBox = new medBrowserSourceSelectorToolBox;
    connect(d->sourceSelectorToolBox, &medBrowserSourceSelectorToolBox::indexChanged,
            this, &medBrowserAreaCentralWidget::_changeSource);

    // Toolbox container
    d->toolboxContainer = new medToolBoxContainer;
    d->toolboxContainer->setObjectName("browserContainerToolbox");
    d->toolboxContainer->setFixedWidth(340);
    d->toolboxContainer->addToolBox(d->sourceSelectorToolBox);

    // Layout /////////////////////////////////////////////
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->toolboxContainer);
    layout->addWidget(d->stack);

    // make toolboxes visible
    this->_changeSource(d->stack->currentIndex());

    //Check if there are already item in the database, otherwise, switch to File system datasource
    QList<medDataIndex> indexes = medDatabaseNonPersistentController::instance()->availableItems();
    QList<medDataIndex> patients = medDatabaseController::instance()->patients();
    if (indexes.isEmpty() && patients.isEmpty())
    {
        d->sourceSelectorToolBox->setCurrentTab(1);
    }

    //dataSources
    for(medAbstractDatasource *dataSource : medDatasourceManager::instance()->dataSources())
        this->_addDatasource(dataSource);
    this->setLayout(layout);
}

medBrowserAreaCentralWidget::~medBrowserAreaCentralWidget()
{
    delete d;
}

void medBrowserAreaCentralWidget::_changeSource(int index)
{
    this->_setToolBoxesVisible(d->stack->currentIndex(), false);
    this->_setToolBoxesVisible(index, true);
    d->stack->setCurrentIndex(index);
}

void medBrowserAreaCentralWidget::_addDatasource( medAbstractDatasource* dataSource )
{
    d->dataSources.push_back(dataSource);
    d->stack->addWidget(dataSource->mainViewWidget());
    d->sourceSelectorToolBox->addTab(dataSource->tabName(),
                                     dataSource->sourceSelectorWidget(),
                                     dataSource->description());

    QList<medToolBox*> toolBoxes = dataSource->getToolBoxes();
    for(medToolBox* toolBox : toolBoxes)
    {
        toolBox->setVisible(false);
        d->toolboxContainer->addToolBox(toolBox);
    }
    this->_changeSource(d->stack->currentIndex());
}

void medBrowserAreaCentralWidget::_setToolBoxesVisible(int index, bool visible )
{
    if(d->dataSources.isEmpty())
        return;

    QList<medToolBox*> toolBoxes = d->dataSources[index]->getToolBoxes();
    for(medToolBox* toolBox : toolBoxes)
    {
        if(toolBox->parentWidget())
            toolBox->setVisible(visible);
        else toolBox->setVisible(false);
    }
}

medBrowserArea::medBrowserArea(QObject *parent): medAbstractArea(parent)
{

}

medBrowserArea::~medBrowserArea(void)
{

}

QWidget* medBrowserArea::centralWidget() const
{
    medBrowserAreaCentralWidget *centralWidget = new medBrowserAreaCentralWidget;
    return centralWidget;
}

QWidget* medBrowserArea::statusBarWidget() const
{
    return new QWidget;
}


