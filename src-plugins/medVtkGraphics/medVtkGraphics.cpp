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
//#include <QVTKGraphicsItem.h>

#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkLookupTableManager.h>
#include <vtkTransferFunctionPresets.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkPointSet.h>
#include <vtkTextProperty.h>
#include <vtkImageMapToColors.h>
#include <vtkOrientedBoxWidget.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>

#include <QVTKWidget2.h>
#include <vtkImageViewCollection.h>
#include <vtkImageView2D.h>
#include <vtkImageView3D.h>
#include <vtkImageView2DCommand.h>
#include <vtkInteractorStyleImageView2D.h>
#include <vtkInteractorStyleTrackballCamera2.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkImageViewCollection.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

#include <QtGui>


//=============================================================================
// Construct a QVector3d from pointer-to-double
inline QVector3D doubleToQtVector3D ( const double * v )
{
    return QVector3D ( v[0], v[1], v[2] );
}
// Convert a QVector3D to array of double.
inline void qtVector3dToDouble ( const QVector3D & inV , double * outV )
{
    outV[0] = inV.x();
    outV[1] = inV.y();
    outV[2] = inV.z();
}
// Convert QColor to vtk's double[3]. (no alpha)
inline void qtColorToDouble ( const QColor & color, double * cv )
{
    cv[0] = color.redF();
    cv[1] = color.greenF();
    cv[2] = color.blueF();
}


class v3dViewObserver : public vtkCommand
{
public:
    static v3dViewObserver* New()
    {
        return new v3dViewObserver;
    }

    void Execute ( vtkObject *caller, unsigned long event, void *callData );

    void setSlider ( QSlider *slider )
    {
        this->slider = slider;
    }

    void setView ( medVtkGraphics *view )
    {
        this->view = view;
    }

    inline void   lock()
    {
        this->m_lock = 1;
    }
    inline void unlock()
    {
        this->m_lock = 0;
    }

protected:
    v3dViewObserver();
    ~v3dViewObserver();

private:
    int             m_lock;
    QSlider        *slider;
    medVtkGraphics        *view;
};

v3dViewObserver::v3dViewObserver()
{
    this->slider = 0;
    this->m_lock = 0;
}

v3dViewObserver::~v3dViewObserver()
{

}

void v3dViewObserver::Execute ( vtkObject *caller, unsigned long event, void *callData )
{
    if ( this->m_lock )
        return;

    if ( !this->slider || !this->view )
        return;

    switch ( event )
    {
    case vtkImageView::CurrentPointChangedEvent:
    {
        {
            dtkSignalBlocker blocker ( this->slider );
            unsigned int zslice = this->view->view2d()->GetSlice();
            this->slider->setValue ( zslice );
            this->slider->update();
        }

        const double *pos = this->view->currentView()->GetCurrentPoint();
        QVector3D qpos ( doubleToQtVector3D ( pos ) );
        this->view->emitViewPositionChangedEvent ( qpos );
    }
    break;

    case vtkImageView2DCommand::CameraZoomEvent:
    {
        double zoom = this->view->currentView()->GetZoom();
        this->view->emitViewZoomChangedEvent ( zoom );
    }
    break;

    case vtkImageView2DCommand::CameraPanEvent:
    {
        const double *pan = this->view->view2d()->GetPan();
        QVector2D qpan ( pan[0],pan[1] );
        this->view->emitViewPanChangedEvent ( qpan );
    }
    break;

    case vtkImageView::WindowLevelChangedEvent:
    {
        double level = this->view->currentView()->GetColorLevel();
        double window = this->view->currentView()->GetColorWindow();

        this->view->emitViewWindowingChangedEvent ( level, window );
    }
    break;

    case vtkCommand::InteractionEvent:
    {
        double *pos = this->view->renderer3d()->GetActiveCamera()->GetPosition();
        double *vup = this->view->renderer3d()->GetActiveCamera()->GetViewUp();
        double *foc = this->view->renderer3d()->GetActiveCamera()->GetFocalPoint();
        double   ps = this->view->renderer3d()->GetActiveCamera()->GetParallelScale();

        QVector3D position ( doubleToQtVector3D ( pos ) );
        QVector3D viewup ( doubleToQtVector3D ( vup ) );
        QVector3D focal ( doubleToQtVector3D ( foc ) );

        this->view->emitViewCameraChangedEvent ( position, viewup, focal, ps );
    }

    break;
    }
}


// /////////////////////////////////////////////////////////////////
// medVtkGraphicsPrivate
// /////////////////////////////////////////////////////////////////

class medVtkGraphicsPrivate
{
public:
    medVtkGraphicsView    *graphicsView;
    QVTKWidget2 *vtkWidget;

    vtkRenderer *renderer2d;
    vtkRenderer *renderer3d;
    
    vtkSmartPointer<vtkRenderer> overlayRenderer2d;
    vtkImageView2D *view2d;
    vtkImageView3D *view3d;

    vtkImageView *currentView;

    vtkImageViewCollection *collection;

    v3dViewObserver *observer;
    
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
    d->vtkWidget = qobject_cast<QVTKWidget2 *> (d->graphicsView->viewport());

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

    // Setting up 3D view
    d->renderer3d = vtkRenderer::New();
    d->renderer3d->GetActiveCamera()->SetPosition ( 0, -1, 0 );
    d->renderer3d->GetActiveCamera()->SetViewUp ( 0, 0, 1 );
    d->renderer3d->GetActiveCamera()->SetFocalPoint ( 0, 0, 0 );

    d->view3d = vtkImageView3D::New();
    d->view3d->SetRenderer ( d->renderer3d );
    d->view3d->SetShowBoxWidget ( 0 );
    d->view3d->SetCroppingModeToOff();
    d->view3d->ShowScalarBarOff();
    d->view3d->GetTextProperty()->SetColor ( 1.0, 1.0, 1.0 );
    d->view3d->ShadeOn();
    
    d->currentView = d->view2d;

    d->renWin = vtkGenericOpenGLRenderWindow::New();
    d->renWin->StereoCapableWindowOn();
    d->renWin->SetStereoTypeToCrystalEyes();

    d->vtkWidget->SetRenderWindow ( d->renWin );
    d->view2d->SetRenderWindow ( d->renWin );
    
    d->collection = vtkImageViewCollection::New();
    d->collection->SetLinkCurrentPoint ( 0 );
    d->collection->SetLinkSliceMove ( 0 );
    d->collection->SetLinkColorWindowLevel ( 0 );
    d->collection->SetLinkCamera ( 0 );
    d->collection->SetLinkZoom ( 0 );
    d->collection->SetLinkPan ( 0 );
    d->collection->SetLinkTimeChange ( 0 );
    d->collection->SetLinkRequestedPosition ( 0 );

    d->collection->AddItem ( d->view2d );
    d->collection->AddItem ( d->view3d );

    d->observer = v3dViewObserver::New();
//    d->observer->setSlider ( d->slider );
    d->observer->setView ( this );

    //d->view2d->GetInteractorStyle()->AddObserver(vtkImageView2DCommand::SliceMoveEvent, d->observer, 0);
    d->view2d->AddObserver ( vtkImageView::CurrentPointChangedEvent, d->observer, 0 );
    d->view2d->AddObserver ( vtkImageView::WindowLevelChangedEvent,  d->observer, 0 );
    d->view2d->GetInteractorStyle()->AddObserver ( vtkImageView2DCommand::CameraZoomEvent, d->observer, 0 );
    d->view2d->GetInteractorStyle()->AddObserver ( vtkImageView2DCommand::CameraPanEvent, d->observer, 0 );
    d->view3d->GetInteractorStyle()->AddObserver ( vtkCommand::InteractionEvent, d->observer, 0 );
    

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
    if ( !d->collection )
        return;

    d->collection->SyncReset();
    
    
    if ( d->currentView )
        d->currentView->GetInteractorStyle()->InvokeEvent ( vtkImageView2DCommand::SliceMoveEvent, NULL );
    
    return;
}

void medVtkGraphics::clear()
{
    d->collection->SyncSetInput ( 0 ); // to be tested
    return;
}

void medVtkGraphics::update()
{
    qDebug()<<"update !!!";
    if ( d->currentView ) {
        d->currentView->Render();
    }
    
    d->vtkWidget->update();
}

void* medVtkGraphics::view()
{
    return d->currentView;
}

vtkImageView2D *medVtkGraphics::view2d()
{
    return d->view2d;
}

vtkImageView3D *medVtkGraphics::view3d()
{
    return d->view3d;
}

vtkImageView *medVtkGraphics::currentView()
{
    return d->currentView;
}

vtkRenderWindowInteractor *medVtkGraphics::interactor()
{
    return d->renWin->GetInteractor();
}

vtkRenderer *medVtkGraphics::renderer2d()
{
    return d->renderer2d;
}

vtkRenderer *medVtkGraphics::renderer3d()
{
    return d->renderer3d;
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

