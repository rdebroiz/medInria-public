/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medDoubleParameter.h>

#include <QSpinBox>
#include <QSlider>

class medDoubleParameterPrivate
{
public:
    double min;
    double max;

    QSpinBox *spinBox;

    ~medDoubleParameterPrivate()
    {
        delete spinBox;
    }
};

medDoubleParameter::medDoubleParameter(QString name, QObject *parent):
    medAbstractDoubleParameter(name,parent),
    d(new medDoubleParameterPrivate)
{
    d->min = 0;
    d->max = 100;
    m_value = 0;
    d->spinBox = NULL;
}

medDoubleParameter::~medDoubleParameter()
{
    delete d;
}

void medDoubleParameter::updateInternWigets()
{

    if(d->spinBox)
        d->spinBox->setValue(m_value);
}

void medDoubleParameter::setRange(double min, double max)
{
    d->min = min;
    d->min = max;

    if(d->spinBox)
        d->spinBox->setRange(min, max);
}


QSpinBox* medDoubleParameter::getSpinBox()
{
    if(!d->spinBox)
    {
        d->spinBox = new QSpinBox;
        d->spinBox->setRange(d->min, d->max);
        d->spinBox->setValue(m_value);

        this->addToInternWidgets(d->spinBox);
        connect(d->spinBox, SIGNAL(destroyed()), this, SLOT(removeInternSlider()));
        connect(d->spinBox, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
    }
    return d->spinBox;
}


QWidget* medDoubleParameter::getWidget()
{
    return this->getSpinBox();
}

void medDoubleParameter::removeInternSpinBox()
{
    this->removeFromInternWidgets(d->spinBox);
    d->spinBox = NULL;
}

