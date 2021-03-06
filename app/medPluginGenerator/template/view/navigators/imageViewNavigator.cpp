/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <%1.h>

#include <medViewFactory.h>

// /////////////////////////////////////////////////////////////////
// %1Private
// /////////////////////////////////////////////////////////////////

class %1Private
{
public:
};

// /////////////////////////////////////////////////////////////////
// %1
// /////////////////////////////////////////////////////////////////

%1::%1(medAbstractView* parent) : medAbstractImageViewNavigator(parent), d(new %1Private)
{
    
}

%1::~%1()
{
    
}

QString %1::identifier() const
{
    return "%1";
}

QString %1::description() const
{
    return "%1";
}

QStringList  %1::handled(void) const
{
    // TODO: view identifiers handled
    return QStringList();
}

bool %1::registered()
{
    medViewFactory *factory = medViewFactory::instance();
    return factory->registerNavigator<%1>("%1", QStringList () << "TODO: view identifier");
}

QList<medAbstractParameter*> %1::linkableParameters()
{
    // TODO: return the parameters that you want to be linkable
    return QList<medAbstractParameter*>();
}

QList<medBoolParameter*> %1::mouseInteractionParameters()
{
    // TODO: return the parameters related to mouse interactions
    return QList<medBoolParameter*>();
}

QWidget* %1::buildToolBoxWidget()
{
    // TODO: construct and return the widget displayed in the
    // view settings toolBox when the container of the parent view is single selected.
    return new QWidget;
}

QWidget* %1::buildToolBarWidget()
{
    // TODO: construct and return the widget displayed in the
    // toolbar of the container where the parent view is displayed.
    return new QWidget;
}

void %1::setZoom (double zoom)
{
    // TODO
}

void %1::setPan (const QVector2D &pan)
{
    // TODO
}

medImageView::Orientation %1::orientation() const
{
    // TODO
    return medImageView::VIEW_ORIENTATION_AXIAL;
}

void %1::setOrientation(medImageView::Orientation orientation)
{
    // TODO
}

void %1::setCamera(QHash<QString,QVariant>)
{
    // TODO
}

void %1::moveToPosition (const QVector3D &position)
{
    // TODO
}
