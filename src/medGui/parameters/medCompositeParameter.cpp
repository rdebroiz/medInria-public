#include "medCompositeParameter.h"

#include <QtGui>


class medCompositeParameterPrivate
{
public:

    QHash<QString, QVariant> variants;
    QHash<QString, QWidget*> widgets;

    ~medCompositeParameterPrivate()
    {
        variants.clear();
        QHash<QString, QWidget*>::iterator i = widgets.begin();
        while (i != widgets.end())
        {
            QWidget* w = i.value();
            delete i.value();
            i++;
        }
        widgets.clear();
    }
};

medCompositeParameter::medCompositeParameter(QString name, QObject* parent):
        medAbstractParameter(name, parent),
        d(new medCompositeParameterPrivate)
{

}

medCompositeParameter::~medCompositeParameter()
{
    delete d;
}

QWidget* medCompositeParameter::getWidget()
{
    QWidget *mainWidget = new QWidget;
    QFormLayout *layout = new QFormLayout(mainWidget);

    QHash<QString, QWidget*>::const_iterator i = d->widgets.constBegin();
    while (i != d->widgets.constEnd())
    {
        layout->addRow(i.key(), i.value());
        ++i;
    }
    return mainWidget;
}

void medCompositeParameter::setValue(const QList<QVariant> value)
{
    if(d->variants.values() == value)
        return;

    if(value.size() != d->variants.values().size())
    {
        qDebug() << "medCompositeParameter::setValue: Wrong number of arguments.";
        return;
    }

    QHash<QString, QVariant>::iterator i = d->variants.begin();
    int index = 0;
    while (i != d->variants.end())
    {
        i.value() = value[index];
        index++;
        i++;
    }

    //  update intern widget
    this->blockInternWidgetsSignals(true);
    this->updateInternWigets();
    this->blockInternWidgetsSignals(false);

    emit valueChanged(value);
}

QList<QVariant> medCompositeParameter::value() const
{
    return d->variants.values();
}

void medCompositeParameter::updateInternWigets()
{
    QHash<QString, QVariant>::const_iterator i = d->variants.constBegin();
    while (i != d->variants.constEnd())
    {
        QString name = i.key();
        QVariant var = i.value();
        QWidget* widget = d->widgets.value(name);

        if(QCheckBox *checkbox = dynamic_cast<QCheckBox*>(widget))
        {
            checkbox->setChecked(var.toBool());
        }
        else if(QSpinBox *spinBox = dynamic_cast<QSpinBox*>(widget))
        {
            spinBox->setValue(var.toInt());
        }
        else if(QDoubleSpinBox *doubleSpinBox = dynamic_cast<QDoubleSpinBox*>(widget))
        {
            doubleSpinBox->setValue(var.toDouble());
        }
        ++i;
    }
}

void medCompositeParameter::addVariant(QString name, QVariant variant, QVariant min, QVariant max, QVariant step)
{
    d->variants.insert(name, variant);

    if(variant.type() == QVariant::Bool)
    {
        QCheckBox *checkbox = new QCheckBox(name);
        d->widgets.insert(name, checkbox);
        addToInternWidgets(checkbox);
        connect(checkbox, SIGNAL(toggled(bool)), this, SLOT(updateValue(bool)));
    }
    else if(variant.type() == QVariant::Int)
    {
        QSpinBox *spinbox = new QSpinBox;
        if(min != QVariant() && max != QVariant())
        {
            spinbox->setMinimum(min.toInt());
            spinbox->setMaximum(max.toInt());
        }
        if(step != QVariant())
        {
            spinbox->setSingleStep(step.toInt());
        }
        spinbox->setValue(variant.toInt());
        d->widgets.insert(name, spinbox);
        addToInternWidgets(spinbox);
        connect(spinbox, SIGNAL(valueChanged(int)), this, SLOT(updateValue(int)));
    }
    else if(variant.type() == QVariant::Double )
    {
        QDoubleSpinBox *spinbox = new QDoubleSpinBox;      
        if(min != QVariant() && max != QVariant())
        {
            spinbox->setMinimum(min.toDouble());
            spinbox->setMaximum(max.toDouble());
        }
        if(step != QVariant())
        {
            spinbox->setSingleStep(step.toDouble());
        }
        spinbox->setValue(variant.toDouble());
        d->widgets.insert(name, spinbox);
        addToInternWidgets(spinbox);
        connect(spinbox, SIGNAL(valueChanged(double)), this, SLOT(updateValue(double)));
    }

    //TODO: to complete with other QVariant types
}


void medCompositeParameter::updateValue(bool value)
{
    QCheckBox *checkbox = dynamic_cast<QCheckBox*>(QObject::sender());
    if(checkbox)
    {
        QString name = d->widgets.key(checkbox);
        d->variants[name] = QVariant(value);
        emit valueChanged(d->variants.values());
    }

}

void medCompositeParameter::updateValue(double value)
{
    QDoubleSpinBox *spinbox = dynamic_cast<QDoubleSpinBox*>(QObject::sender());
    if(spinbox)
    {
        QString name = d->widgets.key(spinbox);
        d->variants[name] = QVariant(value);
        emit valueChanged(d->variants.values());
    }
}

void medCompositeParameter::updateValue(int value)
{
    QSpinBox *spinbox = dynamic_cast<QSpinBox*>(QObject::sender());
    if(spinbox)
    {
        QString name = d->widgets.key(spinbox);
        d->variants[name] = QVariant(value);
        emit valueChanged(d->variants.values());
    }
}