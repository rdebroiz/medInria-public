/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#include "medVtkGraphics.h"


#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>
#include <dtkCore/dtkSignalBlocker.h>
#include <dtkCore/dtkAbstractViewInteractor.h>


//---------------------------------Graphics-----------------------------------
#include "medVtkGraphicsView.h"
#include "medVtkGraphicsScene.h"
#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKGraphicsItem.h>

#include <QtGui>



// /////////////////////////////////////////////////////////////////
// medVtkGraphicsPrivate
// /////////////////////////////////////////////////////////////////

class medVtkGraphicsPrivate
{
public:
    medVtkGraphicsView    *graphicsView;
    medVtkGraphicsScene *graphicsScene;
    QVTKGraphicsItem *vtkWidget;
};


// /////////////////////////////////////////////////////////////////
// medVtkGraphics
// /////////////////////////////////////////////////////////////////

medVtkGraphics::medVtkGraphics() : medAbstractView(), d ( new medVtkGraphicsPrivate )
{

    QMainWindow * mainWindow = dynamic_cast< QMainWindow * >(
        qApp->property( "MainWindow" ).value< QObject * >() );

//    QGLWidget *glWidget = new QGLWidget(mainWindow);
//    glWidget->setFormat(*(new QGLFormat()));
    //    d->graphicsView = new medVtkGraphicsView(glWidget);
//    d->graphicsView = new medVtkGraphicsView(glWidget);

    QGLWidget * viewport = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    viewport->makeCurrent();

    d->graphicsView = new medVtkGraphicsView(mainWindow);
    d->graphicsView->setViewport(viewport);
    d->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    d->graphicsScene = new medVtkGraphicsScene(d->graphicsView);
    d->graphicsView->setScene(d->graphicsScene);

    //QGLContext * glContext = reinterpret_cast<QGLContext*>( glWidget->context() );
    d->vtkWidget = new QVTKGraphicsItem (new QGLContext(QGLFormat()));
    d->graphicsScene->addItem(d->vtkWidget);

    //d->vtkWidget->setSizePolicy ( QSizePolicy::Minimum, QSizePolicy::Minimum );
    //d->vtkWidget->setFocusPolicy ( Qt::NoFocus );

}

medVtkGraphics::~medVtkGraphics()
{
    foreach ( dtkAbstractViewInteractor *interactor, this->interactors() )
    {
        interactor->disable();
        interactor->deleteLater();
    }
}

bool medVtkGraphics::registered()
{
    return dtkAbstractViewFactory::instance()->registerViewType ( medVtkGraphics::s_identifier(), createmedVtkGraphics );
}

QString medVtkGraphics::description() const
{
    return tr ( "A view based on vtkInria3d" );
}


QString medVtkGraphics::identifier() const
{
    return medVtkGraphics::s_identifier();
}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////



QString medVtkGraphics::s_identifier()
{
    return "medVtkGraphics";
}


