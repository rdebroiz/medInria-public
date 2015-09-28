/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractProcessPresenter.h>

#include <QStringList>
#include <QThreadPool>
#include <QPushButton>

#include <medAbstractProcess.h>

class medAbstractProcessPresenterPrivate
{
public:
    QStringList tags;
};

medAbstractProcessPresenter::medAbstractProcessPresenter(QObject *parent): QObject(parent),
    d(new medAbstractProcessPresenterPrivate)
{

}

medAbstractProcessPresenter::~medAbstractProcessPresenter()
{
    delete d;
}

void medAbstractProcessPresenter::addTags(QStringList tags)
{
    d->tags << tags;
}

QStringList medAbstractProcessPresenter::tags() const
{
    return d->tags;
}

QPushButton* medAbstractProcessPresenter::runButton() const
{
    QPushButton *runButton = new QPushButton(tr("Run"));
    connect(runButton, &QPushButton::clicked,
            this, &medAbstractProcessPresenter::runProcessInThread);

    return runButton;
}

void medAbstractProcessPresenter::runProcessInThread()
{
    QThreadPool::globalInstance()->start(this->process());
}

