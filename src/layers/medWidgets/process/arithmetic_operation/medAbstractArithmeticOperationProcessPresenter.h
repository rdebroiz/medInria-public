/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractProcessPresenter.h>
#include <medAbstractArithmeticOperationProcess.h>

#include <medWidgetsExport.h>

class QWidget;
class QPushButton;

class medViewContainerSplitter;
class medAbstractData;

class medAbstractArithmeticOperationProcessPresenterPrivate;
class MEDWIDGETS_EXPORT medAbstractArithmeticOperationProcessPresenter : public medAbstractProcessPresenter
{
    Q_OBJECT

public:
    medAbstractArithmeticOperationProcessPresenter(medAbstractArithmeticOperationProcess *parent);
    virtual ~medAbstractArithmeticOperationProcessPresenter();

    virtual QWidget *buildToolBoxWidget();
    virtual medViewContainerSplitter *buildViewContainerSplitter();

    const QScopedPointer<medAbstractArithmeticOperationProcessPresenterPrivate> d;

private slots:
    // TODO RDE - have to be moved later.
    void _importOutput(medAbstractJob::medJobExitStatus jobExitStatus);
    void _setInput1FromContainer(medAbstractData *data);
    void _setInput2FromContainer(medAbstractData *data);

signals:
    void _outputImported(medAbstractData *);
};

