/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractArithmeticOperationProcessPresenter.h>

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include <medAbstractImageData.h>
#include <medIntParameter.h>
#include <medIntParameterPresenter.h>
#include <medViewContainerSplitter.h>
#include <medViewContainer.h>
#include <medDataManager.h>
#include <medAbstractImageView.h>
#include <medAbstractImageData.h>

class medAbstractArithmeticOperationProcessPresenterPrivate
{
public:
    medAbstractArithmeticOperationProcess *process;
    medIntParameterPresenter *progressionPresenter;
};

medAbstractArithmeticOperationProcessPresenter::medAbstractArithmeticOperationProcessPresenter(medAbstractArithmeticOperationProcess *parent)
    : medAbstractProcessPresenter(parent), d(new medAbstractArithmeticOperationProcessPresenterPrivate)
{
    d->process = parent;
    d->progressionPresenter = new medIntParameterPresenter(d->process->progression());

    connect(d->process, &medAbstractArithmeticOperationProcess::finished,
            this, &medAbstractArithmeticOperationProcessPresenter::_importOutput);
}

medAbstractArithmeticOperationProcessPresenter::~medAbstractArithmeticOperationProcessPresenter()
{
}

QWidget *medAbstractArithmeticOperationProcessPresenter::buildToolBoxWidget()
{
    QWidget *tbWidget = new QWidget;
    QVBoxLayout *tbLayout = new QVBoxLayout;
    tbWidget->setLayout(tbLayout);

    tbLayout->addWidget(this->buildRunButton());
    tbLayout->addWidget(d->progressionPresenter->buildProgressBar());
    tbLayout->addWidget(this->buildCancelButton());

    return tbWidget;
}

medViewContainerSplitter *medAbstractArithmeticOperationProcessPresenter::buildViewContainerSplitter()
{
    medViewContainerSplitter* splitter = new medViewContainerSplitter;
    medViewContainer *input1Container = new medViewContainer;
    splitter->addViewContainer(input1Container);
    connect(input1Container, &medViewContainer::dataAdded,
            this, &medAbstractArithmeticOperationProcessPresenter::_setInput1FromContainer);


    input1Container->setDefaultWidget(new QLabel("INPUT 1"));
    input1Container->setClosingMode(medViewContainer::CLOSE_VIEW);
    input1Container->setUserSplittable(false);
    input1Container->setMultiLayered(false);

    medViewContainer * outputContainer = input1Container->splitVertically();
    outputContainer->setDefaultWidget(new QLabel("OUTPUT"));
    outputContainer->setClosingMode(medViewContainer::CLOSE_VIEW);
    outputContainer->setUserSplittable(false);
    outputContainer->setMultiLayered(false);
    outputContainer->setUserOpenable(false);

    connect(this, SIGNAL(_outputImported(medAbstractData*)),
            outputContainer, SLOT(addData(medAbstractData*)));

    medViewContainer *input2Container = input1Container->splitHorizontally();
    input2Container->setDefaultWidget(new QLabel("INPUT 2"));
    input2Container->setClosingMode(medViewContainer::CLOSE_VIEW);
    input2Container->setUserSplittable(false);
    input2Container->setMultiLayered(false);
    connect(input1Container, &medViewContainer::dataAdded,
            this, &medAbstractArithmeticOperationProcessPresenter::_setInput2FromContainer);

    return splitter;
}


void medAbstractArithmeticOperationProcessPresenter::_setInput1FromContainer(medAbstractData *data)
{
    d->process->setInput1(qobject_cast<medAbstractImageData *>(data));
}

void medAbstractArithmeticOperationProcessPresenter::_setInput2FromContainer(medAbstractData *data)
{
    d->process->setInput2(qobject_cast<medAbstractImageData *>(data));
}

void medAbstractArithmeticOperationProcessPresenter::_importOutput(medAbstractJob::medJobExitStatus jobExitStatus)
{
    if(jobExitStatus == medAbstractJob::MED_JOB_EXIT_SUCCES)
    {
        medDataManager::instance()->importData(d->process->output());
        emit _outputImported(d->process->output());
    }
}
