/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#include "medVtkGraphics.h"

#include <itkExtractImageFilter.h>

#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>
#include <dtkCore/dtkSignalBlocker.h>
#include <dtkCore/dtkAbstractViewInteractor.h>

#include <medCore/medAbstractData.h>
#include <medCore/medAbstractViewCoordinates.h>
#include <medCore/medAbstractDataImage.h>

#include <vtkImageData.h>
#include <vtkImageView2D.h>
#include <vtkRenderer.h>

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


    vtkRenderer *renderer2d;
    vtkSmartPointer<vtkRenderer> overlayRenderer2d;
    vtkImageView2D *view2d;

    vtkImageView *currentView;

    vtkGenericOpenGLRenderWindow *renWin;

    dtkAbstractData *data;
    QMap<int, dtkSmartPointer<dtkAbstractData> > sharedData;
    medAbstractDataImage *imageData;
};


// /////////////////////////////////////////////////////////////////
// medVtkGraphics
// /////////////////////////////////////////////////////////////////

medVtkGraphics::medVtkGraphics() : medAbstractView(), d ( new medVtkGraphicsPrivate )
{

    QMainWindow * mainWindow = dynamic_cast< QMainWindow * >(
        qApp->property( "MainWindow" ).value< QObject * >() );

    QGLWidget * viewport = new QGLWidget(QGLFormat(QGL::SampleBuffers));
    viewport->makeCurrent();

    d->graphicsView = new medVtkGraphicsView(mainWindow);
    d->graphicsView->setViewport(viewport);
    d->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    d->graphicsScene = new medVtkGraphicsScene(d->graphicsView);
    d->graphicsView->setScene(d->graphicsScene);

    d->vtkWidget = new QVTKGraphicsItem (new QGLContext(QGLFormat()));
    d->graphicsScene->addItem(d->vtkWidget); 
    d->graphicsView->setGraphicsWidget(d->vtkWidget);

    d->data       = 0;
    d->imageData  = 0;

    // Setting up 2D view

    d->renderer2d = vtkRenderer::New();
    d->view2d = vtkImageView2D::New();
    d->view2d->SetRenderer ( d->renderer2d );
    d->view2d->SetBackground ( 0.0, 0.0, 0.0 );
    d->view2d->SetLeftButtonInteractionStyle ( vtkInteractorStyleImageView2D::InteractionTypeZoom );
    d->view2d->SetMiddleButtonInteractionStyle ( vtkInteractorStyleImageView2D::InteractionTypePan );
    d->view2d->SetViewOrientation ( vtkImageView2D::VIEW_ORIENTATION_AXIAL );
    d->view2d->CursorFollowMouseOff();
    d->view2d->ShowImageAxisOff();
    d->view2d->ShowScalarBarOff();
    d->view2d->ShowRulerWidgetOn();
    d->overlayRenderer2d = vtkSmartPointer<vtkRenderer>::New();
    d->view2d->SetOverlayRenderer(d->overlayRenderer2d);

    d->currentView = d->view2d;

    d->renWin = vtkGenericOpenGLRenderWindow::New();
    d->renWin->StereoCapableWindowOn();
    d->renWin->SetStereoTypeToCrystalEyes();

    d->vtkWidget->SetRenderWindow ( d->renWin );
    d->view2d->SetRenderWindow ( d->renWin );

    connect ( d->graphicsView, SIGNAL ( destroyed() ), this, SLOT ( widgetDestroyed() ) );
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

void medVtkGraphics::setData(dtkAbstractData *data)
{
    qDebug()<<"setData !!!";
}

void medVtkGraphics::reset()
{
    return;
}

void medVtkGraphics::clear()
{
    return;
}

void medVtkGraphics::update()
{
     qDebug()<<"update !!!";
    if ( d->currentView )
    {
        d->currentView->Render();
    }
    d->vtkWidget->update();
}

void* medVtkGraphics::view()
{
    return d->currentView;
}

template <typename IMAGE>
bool medVtkGraphics::SetViewInput(const char* type,dtkAbstractData* data,const int layer)
{
    if (data->identifier()!=type)
        return false;

    if (IMAGE* image = dynamic_cast<IMAGE*>((itk::Object*)(data->data()))) {
        d->view2d->SetITKInput(image,layer);
    }
    return true;
}

bool medVtkGraphics::SetView(const char* type,dtkAbstractData* data)
{
    if (data->identifier()!=type)
        return false;

    return true;
}

template <typename IMAGE>
bool medVtkGraphics::SetViewInputWithConversion(const char* type,const char* newtype,dtkAbstractData* data,const int layer)
{
    if (data->identifier()!=type)
        return false;

    if (IMAGE* image = dynamic_cast<IMAGE*>((itk::Object*)(data->convert(newtype)->data())))
        d->view2d->SetITKInput(image,layer);

    dtkAbstractView::setData(data);
    qDebug()<<"setData !!!";

    return true;
}

void medVtkGraphics::setData(dtkAbstractData *data, int layer)
{
    if (SetViewInput<itk::Image<char,3> >("itkDataImageChar3",data,layer) ||
            SetViewInput<itk::Image<unsigned char,3> >("itkDataImageUChar3",data,layer) ||
            SetViewInput<itk::Image<short,3> >("itkDataImageShort3",data,layer) ||
            SetViewInput<itk::Image<unsigned short,3> >("itkDataImageUShort3",data,layer) ||
            SetViewInput<itk::Image<int,3> >("itkDataImageInt3",data,layer) ||
            SetViewInput<itk::Image<unsigned,3> >("itkDataImageUInt3",data,layer) ||
            SetViewInput<itk::Image<long,3> >("itkDataImageLong3",data,layer) ||
            SetViewInput<itk::Image<unsigned long,3> >("itkDataImageULong3",data,layer) ||
            SetViewInput<itk::Image<float,3> >("itkDataImageFloat3",data,layer) ||
            SetViewInput<itk::Image<double,3> >("itkDataImageDouble3",data,layer) ||
            SetViewInput<itk::Image<itk::RGBPixel<unsigned char>,3> >("itkDataImageRGB3",data,layer) ||
            SetViewInput<itk::Image<itk::RGBAPixel<unsigned char>,3> >("itkDataImageRGBA3",data,layer) ||
            SetViewInput<itk::Image<itk::Vector<unsigned char,3>,3> >("itkDataImageVector3",data,layer) ||
            SetView("itkDataImageShort4",data) ||
            SetView("itkDataImageInt4",data) ||
            SetView("itkDataImageLong4",data) ||
            SetView("itkDataImageChar4",data) ||
            SetView("itkDataImageUShort4",data) ||
            SetView("itkDataImageUInt4",data) ||
            SetView("itkDataImageULong4",data) ||
            SetView("itkDataImageUChar4",data) ||
            SetView("itkDataImageFloat4",data) ||
            SetView("itkDataImageDouble4",data) ||
            SetViewInputWithConversion<itk::Image<char,3> >("vistalDataImageChar3","itkDataImageChar3",data,layer) ||
            SetViewInputWithConversion<itk::Image<unsigned char,3> >("vistalDataImageUChar3","itkDataImageUChar3",data,layer) ||
            SetViewInputWithConversion<itk::Image<short,3> >("vistalDataImageShort3","itkDataImageShort3",data,layer) ||
            SetViewInputWithConversion<itk::Image<unsigned short,3> >("vistalDataImageUShort3","itkDataImageUShort3",data,layer) ||
            SetViewInputWithConversion<itk::Image<int,3> >("vistalDataImageInt3","itkDataImageInt3",data,layer) ||
            SetViewInputWithConversion<itk::Image<unsigned,3> >("vistalDataImageUInt3","itkDataImageUInt3",data,layer) ||
            SetViewInputWithConversion<itk::Image<float,3> >("vistalDataImageFloat3","itkDataImageFloat3",data,layer) ||
            SetViewInputWithConversion<itk::Image<double,3> >("vistalDataImageDouble3","itkDataImageDouble3",data,layer)) {

    }
    else
    {
        if(vtkImageData *dataset = dynamic_cast<vtkImageData*>((vtkDataObject *)(data->data())))
        {
            d->view2d->SetInput(dataset, 0, layer);
        }
    }
}


void medVtkGraphics::setSharedDataPointer(dtkSmartPointer<dtkAbstractData> data)
{
    if ( !data )
        return;
     int layer = 0, imageLayer = 0;
     d->sharedData[layer] = data;

     this->setData ( data.data(), imageLayer );
}

void* medVtkGraphics::data()
{
    return d->data;
}

QWidget * medVtkGraphics::receiverWidget()
{
    return d->graphicsView;
}

QWidget * medVtkGraphics::widget()
{
    return d->graphicsView;
}

void medVtkGraphics::close()
{
    return;
}




// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////



void medVtkGraphics::onPositionChanged  (const  QVector3D &position)
{
    return;
}

void medVtkGraphics::onZoomChanged      (double zoom)
{
    return;
}

void medVtkGraphics::onPanChanged       (const  QVector2D &pan)
{
    return;
}

void medVtkGraphics::onWindowingChanged (double level, double window)
{
    return;
}

void medVtkGraphics::onCameraChanged    (const  QVector3D &position,
                         const  QVector3D &viewup,
                         const  QVector3D &focal,
                         double parallelScale)
{
    return;
}

void medVtkGraphics::onVisibilityChanged(bool   visible, int layer)
{
    return;
}

void medVtkGraphics::onOpacityChanged   (double opacity, int layer)
{
    return;
}

medAbstractViewCoordinates * medVtkGraphics::coordinates()
{
    return 0;
}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////


QString medVtkGraphics::s_identifier()
{
    return "medVtkGraphics";
}

dtkAbstractView *createmedVtkGraphics()
{
    return new medVtkGraphics;
}

