/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medRegistrationWorkspace.h>

#include <dtkCore/dtkSignalBlocker.h>

#include <medViewFactory.h>
#include <medAbstractView.h>
#include <dtkCore/dtkSmartPointer.h>

#include <medRegistrationSelectorToolBox.h>
#include <medViewContainer.h>
#include <medTabbedViewContainers.h>
#include <medRegistrationSelectorToolBox.h>
#include <medAbstractLayeredView.h>
#include <medParameterPoolManager.h>

#include <medToolBoxFactory.h>

class medRegistrationWorkspacePrivate
{
public:
    medRegistrationSelectorToolBox * registrationToolBox;
    medViewContainer *fixedContainer;
    medViewContainer *movingContainer;
    medViewContainer *fuseContainer;
};

medRegistrationWorkspace::medRegistrationWorkspace(QWidget *parent) : medAbstractWorkspace(parent), d(new medRegistrationWorkspacePrivate)
{
    // -- Registration toolbox --

    d->registrationToolBox = new medRegistrationSelectorToolBox(parent);
    this->addToolBox(d->registrationToolBox);

    this->setUserLayerPoolable(false);
    connect(this->stackedViewContainers(), SIGNAL(currentChanged(int)), this, SLOT(updateUserLayerClosable(int)));
    connect(d->registrationToolBox, SIGNAL(movingDataRegistered(medAbstractData*)), this, SLOT(updateFromRegistrationSuccess(medAbstractData*)));

}

medRegistrationWorkspace::~medRegistrationWorkspace(void)
{
    delete d;
    d = NULL;
}

QString medRegistrationWorkspace::identifier() const
{
    return "Registration";
}

QString medRegistrationWorkspace::description() const
{
    return tr("Registration");
}


void medRegistrationWorkspace::setupViewContainerStack()
{

    //the stack has been instantiated in constructor
    if (!this->stackedViewContainers()->count())
    {
        d->fixedContainer = this->stackedViewContainers()->addContainerInTab(tr("Compare"));
        QLabel *fixedLabel = new QLabel(tr("FIXED"));
        fixedLabel->setAlignment(Qt::AlignCenter);
        d->fixedContainer->setDefaultWidget(fixedLabel);
        d->fixedContainer->setMultiLayered(false);
        d->fixedContainer->setUserClosable(false);
        d->fixedContainer->setUserSplittable(false);
        d->fixedContainer->setUserPoolable(false);

        d->movingContainer = d->fixedContainer->splitVertically();
        QLabel *movingLabel = new QLabel(tr("MOVING"));
        movingLabel->setAlignment(Qt::AlignCenter);
        d->movingContainer->setDefaultWidget(movingLabel);
        d->movingContainer->setUserClosable(false);
        d->movingContainer->setUserSplittable(false);
        d->movingContainer->setMultiLayered(false);
        d->movingContainer->setUserPoolable(false);


        d->fuseContainer = this->stackedViewContainers()->addContainerInTab(tr("Fuse"));
        QLabel *fuseLabel = new QLabel(tr("FUSE"));
        fuseLabel->setAlignment(Qt::AlignCenter);
        d->fuseContainer->setDefaultWidget(fuseLabel);
        d->fuseContainer->setUserClosable(false);
        d->fuseContainer->setUserSplittable(false);
        d->fuseContainer->setAcceptDrops(false);
        d->fuseContainer->setUserPoolable(false);

        connect(d->fixedContainer, SIGNAL(viewContentChanged()),
                this, SLOT(updateFromFixedContainer()));
        connect(d->movingContainer,SIGNAL(viewContentChanged()),
                this, SLOT(updateFromMovingContainer()));

        connect(d->fixedContainer,SIGNAL(viewRemoved()),
                this, SLOT(updateFromFixedContainer()));
        connect(d->movingContainer,SIGNAL(viewRemoved()),
                this, SLOT(updateFromMovingContainer()));

        this->stackedViewContainers()->lockTabs();
        this->stackedViewContainers()->setCurrentIndex(0);
    }
}

void medRegistrationWorkspace::patientChanged(int patientId)
{
    d->registrationToolBox->clear();
}

bool medRegistrationWorkspace::isUsable()
{
    medToolBoxFactory * tbFactory = medToolBoxFactory::instance();
    return (tbFactory->toolBoxesFromCategory("registration").size()!=0); 
}

void medRegistrationWorkspace::updateFromMovingContainer()
{
    if(!d->movingContainer->view())
    {
        medAbstractLayeredView* fuseView  = dynamic_cast<medAbstractLayeredView*>(d->fuseContainer->view());
        if(fuseView)
        {
            if(fuseView->layer(d->registrationToolBox->movingData()) == 0)
            {
                d->fuseContainer->setView(NULL);
                d->fuseContainer->addData(d->registrationToolBox->fixedData());
            }
            else
                fuseView->removeLayer(1);

        }

        d->registrationToolBox->setMovingData(NULL);
        return;
    }

    medAbstractLayeredView* movingView  = dynamic_cast<medAbstractLayeredView*>(d->movingContainer->view());
    if(!movingView)
    {
        qWarning() << "Non layered view are not suported yet in registration workspace.";
        return;
    }

    medAbstractData *movingData = movingView->layerData(movingView->currentLayer());

    medAbstractLayeredView* fuseView  = dynamic_cast<medAbstractLayeredView*>(d->fuseContainer->view());
    if(fuseView)
        fuseView->removeData(movingData);

    d->fuseContainer->addData(movingData);

    d->movingContainer->link("1");
    d->fuseContainer->link("1");

    foreach(medAbstractInteractor *intercator, movingView->interactors(0))
        foreach (medAbstractParameter *parameter, intercator->parameters())
            medParameterPoolManager::instance()->linkParameter(parameter, "movingIntercators");

    fuseView  = dynamic_cast<medAbstractLayeredView*>(d->fuseContainer->view());
    foreach(medAbstractInteractor *intercator, fuseView->interactors(fuseView->layer(movingData)))
        foreach (medAbstractParameter *parameter, intercator->parameters())
            medParameterPoolManager::instance()->linkParameter(parameter, "movingIntercators");

    d->registrationToolBox->setMovingData(movingData);
}

void medRegistrationWorkspace::updateFromFixedContainer()
{
    if(!d->fixedContainer->view())
    {
        medAbstractLayeredView* fuseView  = dynamic_cast<medAbstractLayeredView*>(d->fuseContainer->view());
        if(fuseView)
        {
            if(fuseView->layer(d->registrationToolBox->fixedData()) == 0)
            {
                d->fuseContainer->setView(NULL);
                d->fuseContainer->addData(d->registrationToolBox->movingData());
            }
            else
                fuseView->removeLayer(1);

        }

        d->registrationToolBox->setFixedData(NULL);
        return;
    }

    medAbstractLayeredView* fixedView  = dynamic_cast<medAbstractLayeredView*>(d->fixedContainer->view());
    if(!fixedView)
    {
        qWarning() << "Non layered view are not suported yet in registration workspace.";
        return;
    }

    medAbstractData *fixedData = fixedView->layerData(fixedView->currentLayer());
    medAbstractLayeredView* fuseView  = dynamic_cast<medAbstractLayeredView*>(d->fuseContainer->view());
    if(fuseView)
        fuseView->removeData(fixedData);

    d->fuseContainer->addData(fixedData);

    d->fixedContainer->link("1");
    d->fuseContainer->link("1");

    foreach(medAbstractInteractor *intercator, fixedView->interactors(0))
        foreach (medAbstractParameter *parameter, intercator->parameters())
            medParameterPoolManager::instance()->linkParameter(parameter, "fixedIntercators");

    fuseView  = dynamic_cast<medAbstractLayeredView*>(d->fuseContainer->view());
    foreach(medAbstractInteractor *intercator, fuseView->interactors(fuseView->layer(fixedData)))
        foreach (medAbstractParameter *parameter, intercator->parameters())
            medParameterPoolManager::instance()->linkParameter(parameter, "fixedIntercators");

    d->registrationToolBox->setFixedData(fixedData);
}


void medRegistrationWorkspace::updateUserLayerClosable(int tabIndex)
{
    if(tabIndex == 0)
        this->setUserLayerClosable(true);
    else
        this->setUserLayerClosable(false);
}

void medRegistrationWorkspace::updateFromRegistrationSuccess(medAbstractData *output)
{
    //TODO disconnect because we dont want to change input of the undo redo process.
    //  find a better way to do it ? - RDE
    d->movingContainer->disconnect(this);

    d->movingContainer->setView(NULL);
    d->movingContainer->addData(output);

    d->fuseContainer->setView(NULL);
    d->fuseContainer->addData(d->registrationToolBox->fixedData());
    d->fuseContainer->addData(output);

    connect(d->movingContainer,SIGNAL(viewContentChanged()),
            this, SLOT(updateFromMovingContainer()));

    connect(d->movingContainer,SIGNAL(viewRemoved()),
            this, SLOT(updateFromMovingContainer()));
}
