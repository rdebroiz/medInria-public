// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include <vtkINRIA3DConfigure.h>

#ifdef vtkINRIA3D_USE_ITK
#include "medItk.h"
#endif

#include "v3dView2D.h"
#include "v3dView3D.h"
#include "v3dViewWidget.h"

#include <dtkCore/dtkAbstractViewFactory.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkViewImage2D.h>
#include <vtkViewImage3D.h>

#include <QVTKWidget.h>

// /////////////////////////////////////////////////////////////////
// v3dViewPrivate
// /////////////////////////////////////////////////////////////////

class v3dView3DPrivate
{
public:
    vtkRenderer *renderer;
    vtkViewImage3D *view;

    v3dViewWidget *widget;
};

// /////////////////////////////////////////////////////////////////
// v3dView3D
// /////////////////////////////////////////////////////////////////

v3dView3D::v3dView3D(void) : dtkAbstractView(), d(new v3dView3DPrivate)
{
    d->renderer = vtkRenderer::New();
    
    d->widget = new v3dViewWidget;
    d->widget->GetRenderWindow()->AddRenderer(d->renderer);

    d->view = vtkViewImage3D::New();
    d->view->SetRenderWindow(d->widget->GetRenderWindow());
    d->view->SetRenderer(d->renderer);
    double textcolor[3] = {0.0, 0.0, 0.0};
    d->view->SetTextColor(textcolor);
    d->view->SetRenderingModeToPlanar();
    d->view->SetBoxWidgetVisibility(0);
}

v3dView3D::~v3dView3D(void)
{

}

bool v3dView3D::registered(void)
{
    return dtkAbstractViewFactory::instance()->registerViewType("v3dView3D", createV3dView3D);
}

QString v3dView3D::description(void) const
{
    return "v3dView3D";
}

// /////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////

void v3dView3D::reset(void)
{
    if(!d->view)
	return;
    
    d->view->Reset();
}

void v3dView3D::update(void)
{
    d->view->Render();

    d->widget->update();
}

void v3dView3D::link(dtkAbstractView *other)
{
    if(!d->view)
	return;

    if(vtkSynchronizedView *view = dynamic_cast<vtkSynchronizedView *>((vtkObject*)(other->view())))
	d->view->AddChild(view);
    /*
    if(vtkViewImage3D *view = dynamic_cast<vtkViewImage3D*>((vtkObject*)(other->view())))
	d->view->AddChild(view);
    */
}

void v3dView3D::unlink(dtkAbstractView *other)
{
    Q_UNUSED(other);
}

void *v3dView3D::view(void)
{
    return d->view;
}

void v3dView3D::setData(dtkAbstractData *data)
{
    if(!d->view)
	return;

    if(!data)
	return;

#ifdef vtkINRIA3D_USE_ITK
    if( itk::Image<char, 3>* image = dynamic_cast<itk::Image<char, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<unsigned char, 3>* image = dynamic_cast<itk::Image<unsigned char, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<short, 3>* image = dynamic_cast<itk::Image<short, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<unsigned short, 3>* image = dynamic_cast<itk::Image<unsigned short, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<int, 3>* image = dynamic_cast<itk::Image<int, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<unsigned int, 3>* image = dynamic_cast<itk::Image<unsigned int, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<long, 3>* image = dynamic_cast<itk::Image<long, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<unsigned long, 3>* image = dynamic_cast<itk::Image<unsigned long, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<float, 3>* image = dynamic_cast<itk::Image<float, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
    else if( itk::Image<double, 3>* image = dynamic_cast<itk::Image<double, 3>*>( (itk::Object*)( data->data() ) ) )
      d->view->SetITKImage ( image );
#endif

    if(vtkImageData* image = dynamic_cast<vtkImageData*>((vtkDataObject *)(data->data())))
        d->view->SetImage(image);
    
    dtkAbstractView::setData(data);
}

QWidget *v3dView3D::widget(void)
{
    return d->widget;
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractView *createV3dView3D(void)
{
    return new v3dView3D;
}
