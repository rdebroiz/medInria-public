/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medTestExport.h>

#include <dtkCore/dtkSmartPointer.h>

#include <medAbstractLayeredView.h>

class medAbstractData;
class medQtViewPrivate;

class MEDTEST_EXPORT medQtView : public medAbstractLayeredView
{
    Q_OBJECT

public:
             medQtView(QObject* parent = 0);
    virtual ~medQtView();

    virtual QString description() const;

    static QString s_description();
    static bool registered();

    QWidget *widget();
    virtual medAbstractLayeredViewInteractor * primaryInteractor(medAbstractData* data);
    virtual QList<medAbstractInteractor *> extraInteractors(medAbstractData* data);
    virtual medAbstractLayeredViewInteractor * primaryInteractor(unsigned int layer);
    virtual QList<medAbstractInteractor *> extraInteractors(unsigned int layer);

    virtual medAbstractLayeredViewNavigator * primaryNavigator();
    virtual QList<medAbstractNavigator *> extraNavigators();

    virtual QWidget *viewWidget();
    virtual QWidget* toolBar();
    virtual QWidget* toolBox();

    virtual void initialiseInteractors(medAbstractData* data);
    virtual void initialiseNavigators();

    virtual medViewBackend * backend() const;

    virtual void removeInteractors(medAbstractData *data);

    virtual QList<medAbstractParameter*> viewParameters(){}
    virtual QList<medAbstractParameter*> navigatorsParameters(){}


protected:

private:
    medQtViewPrivate *d;
};


