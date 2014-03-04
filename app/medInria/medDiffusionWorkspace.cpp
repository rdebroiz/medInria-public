/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medDiffusionWorkspace.h>

#include <dtkCore/dtkSmartPointer.h>
#include <medAbstractData.h>


#include <medDataManager.h>

#include <medViewContainer.h>
#include <medTabbedViewContainers.h>
#include <medToolBox.h>
#include <medToolBoxFactory.h>
#include <medDiffusionSelectorToolBox.h>

#include <medAbstractView.h>
#include <medAbstractImageView.h>

class medDiffusionWorkspacePrivate
{
public:

    medToolBox * diffusionToolBox;
    medToolBox * fiberBundlingToolBox;
    medViewContainer * diffusionContainer;
};

medDiffusionWorkspace::medDiffusionWorkspace(QWidget *parent) : medAbstractWorkspace(parent), d(new medDiffusionWorkspacePrivate)
{
//    d->diffusionContainer = 0;

//    d->diffusionToolBox =  new medDiffusionSelectorToolBox(parent);

//    d->fiberBundlingToolBox = medToolBoxFactory::instance()->createToolBox("medFiberBundlingToolBox", parent);

//    connect(d->diffusionToolBox, SIGNAL(addToolBox(medToolBox *)),
//            this, SLOT(addToolBox(medToolBox *)));
//    connect(d->diffusionToolBox, SIGNAL(removeToolBox(medToolBox *)),
//            this, SLOT(removeToolBox(medToolBox *)));

//    connect(d->diffusionToolBox, SIGNAL(newOutput(medAbstractData*)), d->fiberBundlingToolBox, SLOT(setInput(medAbstractData*)));
//    connect(d->diffusionToolBox, SIGNAL(newOutput(medAbstractData*)), this, SLOT(addToView(medAbstractData*)));

//    // -- View toolboxes --

//    QList<QString> toolboxNames = medToolBoxFactory::instance()->toolBoxesFromCategory("view");
//    if(toolboxNames.contains("medViewPropertiesToolBox"))
//    {
//        // we want the medViewPropertiesToolBox to be the first "view" toolbox
//        toolboxNames.move(toolboxNames.indexOf("medViewPropertiesToolBox"),0);
//    }
//    foreach(QString toolbox, toolboxNames)
//    {
//       this->addToolBox(medToolBoxFactory::instance()->createToolBox(toolbox, parent));
//    }

//    this->addToolBox( d->diffusionToolBox );
//    this->addToolBox( d->fiberBundlingToolBox );

}

medDiffusionWorkspace::~medDiffusionWorkspace()
{
    delete d;
    d = NULL;
}

QString medDiffusionWorkspace::identifier() const {
    return "Diffusion";
}

QString medDiffusionWorkspace::description() const {
    return "Diffusion";
}

void medDiffusionWorkspace::setupViewContainerStack()
{
//    //the stack has been instantiated in constructor
//    if ( ! this->stackedViewContainers()->count())
//    {
//        medViewContainer *viewContainer = new medViewContainer();

//        //ownership of singleViewContainer is transferred to the stackedWidget.
//        this->stackedViewContainers()->addContainerInTab (identifier());

//        d->diffusionContainer = viewContainer;

//        this->stackedViewContainers()->lockTabs();
//        this->stackedViewContainers()->hideTabBar();
//    }
//    else
//    {
//        d->diffusionContainer = this->currentViewContainer();
//        //TODO: maybe clear views here too?
//    }

//    if ( ! d->diffusionContainer)
//        return;
}

void medDiffusionWorkspace::addToView(medAbstractData * data)
{
    medAbstractImageView* view = dynamic_cast<medAbstractImageView*>(d->diffusionContainer->view());
    if(!view)
        return;

    view->addLayer(data);
}

void medDiffusionWorkspace::onAddTabClicked()
{
    this->stackedViewContainers()->addContainerInTab(this->description());
}

bool medDiffusionWorkspace::isUsable()
{
    medToolBoxFactory * tbFactory = medToolBoxFactory::instance();
    return (tbFactory->toolBoxesFromCategory("diffusion").size()!=0); 
}
