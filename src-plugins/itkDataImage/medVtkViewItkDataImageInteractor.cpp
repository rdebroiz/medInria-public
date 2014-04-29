/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medVtkViewItkDataImageInteractor.h>

#include <QVTKWidget.h>

#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>

#include <vtkImageView2D.h>
#include <vtkImageView3D.h>
#include <vtkTransferFunctionPresets.h>
#include <vtkLookupTableManager.h>
#include <vtkImageViewCollection.h>
#include <vtkRenderWindow.h>
#include <vtkImageData.h>

#include <medVtkViewBackend.h>
#include <medAbstractData.h>
#include <medViewFactory.h>
#include <medAbstractImageView.h>
#include <medStringListParameter.h>
#include <medIntParameter.h>
#include <medBoolParameter.h>
#include <medDoubleParameter.h>
#include <medVector3DParameter.h>
#include <medAbstractImageData.h>
#include <medCompositeParameter.h>

#include <QFormLayout>



class medVtkViewItkDataImageInteractorPrivate
{
public:
    // views
    vtkImageView2D *view2d;
    vtkImageView3D *view3d;
    vtkImageViewCollection *collection;
    medAbstractImageData *imageData;
    vtkRenderWindow *render;
    vtkRenderer *renderer2d;

    medAbstractImageView *medVtkView;

    medStringListParameter *lutParam;
    medStringListParameter *presetParam;
    medIntParameter *opacityParam;
    medBoolParameter *visibiltyParameter;
    medCompositeParameter *windowLevelParameter;
    medIntParameter *slicingParameter;
};


medVtkViewItkDataImageInteractor::medVtkViewItkDataImageInteractor(medAbstractView *parent):
    medAbstractImageViewInteractor(parent), d(new medVtkViewItkDataImageInteractorPrivate)
{
    d->medVtkView = dynamic_cast<medAbstractImageView*>(parent);
    medVtkViewBackend* backend = static_cast<medVtkViewBackend*>(parent->backend());
    d->view2d = backend->view2D;
    d->view3d = backend->view3D;
    d->collection = vtkImageViewCollection::New();
    d->collection->AddItem(d->view2d);
    d->collection->AddItem(d->view3d);
    d->render = backend->renWin;
    d->renderer2d = backend->renderer2D;
    d->imageData = NULL;

    d->lutParam = NULL;
    d->presetParam = NULL;
    d->opacityParam = NULL;
    d->visibiltyParameter = NULL;
    d->slicingParameter = NULL;
    d->windowLevelParameter = NULL;
}

medVtkViewItkDataImageInteractor::~medVtkViewItkDataImageInteractor()
{
    delete d;
    d = NULL;
}

QStringList medVtkViewItkDataImageInteractor::dataHandled()
{
    QStringList d = QStringList() << "itkDataImageChar3"
                                  << "itkDataImageUChar3"
                                  << "itkDataImageShort3"
                                  << "itkDataImageUShort3"
                                  << "itkDataImageInt3"
                                  << "itkDataImageLong3"
                                  << "itkDataImageULong3"
                                  << "itkDataImageFloat3"
                                  << "itkDataImageDouble3"
                                  << "itkDataImageRGB3"
                                  << "itkDataImageRGBA3"
                                  << "itkDataImageVector3";
    return  d;
}

QStringList medVtkViewItkDataImageInteractor::handled() const
{
    return medVtkViewItkDataImageInteractor::dataHandled();
}

QString medVtkViewItkDataImageInteractor::description() const
{
    return tr("Interactor for ITK images");
}


QString medVtkViewItkDataImageInteractor::identifier() const
{
    return "medVtkViewItkDataImageInteractor";
}

bool medVtkViewItkDataImageInteractor::registered()
{
    medViewFactory *factory = medViewFactory::instance();
    return factory->registerInteractor<medVtkViewItkDataImageInteractor>("medVtkViewItkDataImageInteractor",
                                                                  QStringList () << "medVtkView" <<
                                                                  medVtkViewItkDataImageInteractor::dataHandled());
}

QList<medAbstractParameter*> medVtkViewItkDataImageInteractor::parameters()
{
    QList<medAbstractParameter*> params;
    params.append(d->lutParam);
    params.append(d->presetParam);
    params.append(d->opacityParam);
    params.append(d->visibiltyParameter);
    params.append(d->windowLevelParameter);

    return params;
}


void medVtkViewItkDataImageInteractor::setData(medAbstractData *data)
{
    d->imageData = dynamic_cast<medAbstractImageData *>(data);
    if(!d->imageData)
        return;

    if (!(SetViewInput<itk::Image<char,3> >("itkDataImageChar3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<unsigned char,3> >("itkDataImageUChar3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<short,3> >("itkDataImageShort3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<unsigned short,3> >("itkDataImageUShort3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<int,3> >("itkDataImageInt3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<unsigned,3> >("itkDataImageUInt3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<long,3> >("itkDataImageLong3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<unsigned long,3> >("itkDataImageULong3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<float,3> >("itkDataImageFloat3", data , d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<double,3> >("itkDataImageDouble3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<itk::RGBPixel<unsigned char>,3> >("itkDataImageRGB3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<itk::RGBAPixel<unsigned char>,3> >("itkDataImageRGBA3", data, d->medVtkView->layer(data)) ||
          SetViewInput<itk::Image<itk::Vector<unsigned char,3>,3> >("itkDataImageVector3", data, d->medVtkView->layer(data))))
    {
        qDebug() << "Unable to add data: " << data->identifier() << " to view " << this->identifier();
        return;
    }

    initParameters(d->imageData);
}

void medVtkViewItkDataImageInteractor::removeData()
{
    d->view2d->RemoveLayer(d->medVtkView->layer(d->imageData));
    d->view3d->RemoveLayer(d->medVtkView->layer(d->imageData));
    if(d->medVtkView->is2D())
        d->view2d->Render();
    else
        d->view3d->Render();
}


template <typename IMAGE>
bool medVtkViewItkDataImageInteractor::SetViewInput(const char* type, medAbstractData* data, int layer)
{
    if (data->identifier() != type)
        return false;

    if (IMAGE* image = dynamic_cast<IMAGE*>((itk::Object*)(data->data())))
    {
        d->view2d->SetITKInput(image, layer);
        d->view3d->SetITKInput(image, layer);

        return true;
    }
    return false;
}

void medVtkViewItkDataImageInteractor::initParameters(medAbstractImageData* data)
{
    d->imageData = data;

    d->lutParam = new medStringListParameter("Lut", this);
    QStringList lut = QStringList() << "Default" << "Black & White" << "Black & White Inversed"
                                     << "Spectrum" << "Hot Metal" << "Hot Green"
                                     << "Hot Iron" << "GE" << "Flow" << "Loni" << "Loni 2"
                                     << "Asymmetry" << "P-Value" << "Red Black Alpha"
                                     << "Green Black Alpha" << "Blue Black Alpha"
                                     << "Muscles & Bones" << "Bones" << "Red Vessels"
                                     << "Cardiac" << "Gray Rainbow" << "Stern" << "Black Body";
    d->lutParam->addItems(lut);
    connect(d->lutParam, SIGNAL(valueChanged(QString)), this, SLOT(setLut(QString)));


    d->presetParam = new medStringListParameter("Preset", this);
    QStringList presets = QStringList() << "None" << "VR Muscles&Bones" << "Vascular I"
                                        << "Vascular II" << "Vascular III" << "Vascular IV"
                                        << "Standard" << "Soft" << "Soft on White"
                                        << "Soft on Blue" << "Red on White" << "Glossy" ;
    d->presetParam->addItems(presets);

    connect(d->presetParam, SIGNAL(valueChanged(QString)), this, SLOT(setPreset(QString)));

    d->opacityParam = new medIntParameter("Opacity", this);
    d->opacityParam->setRange(0, 100);
    d->opacityParam->setValue(100);
    connect(d->opacityParam, SIGNAL(valueChanged(int)), this, SLOT(setOpacity(int)));

    d->visibiltyParameter = new medBoolParameter("Visibility", this);
    d->visibiltyParameter->setValue(true);
    connect(d->visibiltyParameter, SIGNAL(valueChanged(bool)), this, SLOT(setVisibility(bool)));

    d->view2d->GetInput()->Update();
    double* range = d->view2d->GetInput(d->medVtkView->layer(data))->GetScalarRange();
    double window = range[1]-range[0];
    double level = 0.5*(range[1]+range[0]);

    d->windowLevelParameter = new medCompositeParameter("WindowLevel", this);
    connect(d->windowLevelParameter, SIGNAL(valueChanged(QList<QVariant>)), this, SLOT(setWindowLevel(QList<QVariant>)));
    d->windowLevelParameter->addVariant("Window", QVariant(window), QVariant(range[0]), QVariant(range[1]));
    d->windowLevelParameter->addVariant("Level", QVariant(level), QVariant(range[0]), QVariant(range[1]));

    //TODO GPR-RDE: Shouldn't it be a navigator parameter?
    d->slicingParameter = new medIntParameter("Slicing", this);
    // slice orientation may differ from view orientation. Adapt slider range accordingly.
    int orientationId = d->view2d->GetSliceOrientation();
    if (orientationId==vtkImageView2D::SLICE_ORIENTATION_XY)
        d->slicingParameter->setRange(0, d->imageData->zDimension()-1);
    else if (orientationId==vtkImageView2D::SLICE_ORIENTATION_XZ)
        d->slicingParameter->setRange (0, d->imageData->yDimension()-1);
    else if (orientationId==vtkImageView2D::SLICE_ORIENTATION_YZ)
        d->slicingParameter->setRange (0, d->imageData->xDimension()-1);

    connect(d->slicingParameter, SIGNAL(valueChanged(int)), this, SLOT(moveToSlice(int)));

    connect(d->medVtkView, SIGNAL(sliceChanged(int)), d->slicingParameter, SLOT(setValue(int)) );
    connect(d->medVtkView, SIGNAL(windowLevelChanged(double,double, unsigned int)), this, SLOT(updateWindowLevelParam(double,double, unsigned int)) );
    connect(d->medVtkView, SIGNAL(currentLayerChanged()), this, SLOT(updateImageViewInternalLayer()));

    if(d->medVtkView->layer(d->imageData) == 0)
    {
        switch(d->view2d->GetViewOrientation())
        {
        case vtkImageView2D::VIEW_ORIENTATION_AXIAL:
            d->medVtkView->setOrientation(medImageView::VIEW_ORIENTATION_AXIAL);
            break;
        case vtkImageView2D::VIEW_ORIENTATION_SAGITTAL:
            d->medVtkView->setOrientation(medImageView::VIEW_ORIENTATION_SAGITTAL);
            break;
        case vtkImageView2D::VIEW_ORIENTATION_CORONAL:
            d->medVtkView->setOrientation(medImageView::VIEW_ORIENTATION_CORONAL);
            break;
        }
    }
}

void medVtkViewItkDataImageInteractor::setOpacity (int opacity)
{
    double value = (double)(opacity) / 100.0;
    setOpacity(value);
}

void medVtkViewItkDataImageInteractor::setOpacity(double opacity)
{
    d->view3d->SetOpacity (opacity, d->medVtkView->layer(d->imageData));
    d->view2d->SetOpacity (opacity, d->medVtkView->layer(d->imageData));

    update();
}

double medVtkViewItkDataImageInteractor::opacity() const
{
     double opacity = static_cast<double>(d->opacityParam->value()) / 100.0;
     return opacity;
}

void medVtkViewItkDataImageInteractor::setVisibility(bool visible)
{
    if(visible)
    {
        d->view2d->SetVisibility(1, d->medVtkView->layer(d->imageData));
        d->view3d->SetVisibility(1, d->medVtkView->layer(d->imageData));
    }
    else
    {
        d->view2d->SetVisibility(0, d->medVtkView->layer(d->imageData));
        d->view3d->SetVisibility(0, d->medVtkView->layer(d->imageData));
    }

    update();
}

bool medVtkViewItkDataImageInteractor::visibility() const
{
    return d->visibiltyParameter->value();
}

QString medVtkViewItkDataImageInteractor::lut() const
{
    return d->lutParam->value();
}

void medVtkViewItkDataImageInteractor::setLut(QString value)
{
    typedef vtkTransferFunctionPresets Presets;
    vtkColorTransferFunction * rgb   = vtkColorTransferFunction::New();
    vtkPiecewiseFunction     * alpha = vtkPiecewiseFunction::New();

    Presets::GetTransferFunction(value.toStdString(), rgb, alpha );

    if (d->medVtkView->layer(d->imageData) == 0)
        d->view2d->SetTransferFunctions(rgb, alpha, d->medVtkView->layer(d->imageData));
    else
    {
        //TODO: find out why its not the same process - RDE
        vtkLookupTable *lut = vtkLookupTableManager::GetLookupTable(value.toStdString());
        d->view2d->SetLookupTable(lut, d->medVtkView->layer(d->imageData));
        lut->Delete();
    }

    d->view3d->SetTransferFunctions(rgb, alpha, d->medVtkView->layer(d->imageData));
    d->view3d->SetLookupTable(vtkLookupTableManager::GetLookupTable(value.toStdString()), d->medVtkView->layer(d->imageData));

    rgb->Delete();
    alpha->Delete();

    update();
}

void medVtkViewItkDataImageInteractor::setPreset(QString preset)
{
    //TODO to complete wat for theo stuff ?) - RDE
    DTK_UNUSED(preset);
}

QString medVtkViewItkDataImageInteractor::preset() const
{
  return d->presetParam->value();
}


QWidget* medVtkViewItkDataImageInteractor::buildToolBarWidget()
{
    d->slicingParameter->getSlider()->setOrientation(Qt::Horizontal);
    return d->slicingParameter->getSlider();
}

QWidget* medVtkViewItkDataImageInteractor::buildToolBoxWidget()
{

    QWidget *toolbox = new QWidget;
    QFormLayout *layout = new QFormLayout(toolbox);
    layout->addRow(d->windowLevelParameter->getWidget());
    layout->addRow(d->lutParam->getLabel(), d->lutParam->getComboBox());
    layout->addRow(d->presetParam->getLabel(), d->presetParam->getComboBox());

    return toolbox;
}

QWidget* medVtkViewItkDataImageInteractor::buildLayerWidget()
{
    d->opacityParam->getSlider()->setOrientation(Qt::Horizontal);
    return d->opacityParam->getSlider();
}

void medVtkViewItkDataImageInteractor::setWindowLevel (double &window, double &level)
{
    QList<QVariant> value;
    value.append(QVariant(window));
    value.append(QVariant(level));
    d->windowLevelParameter->setValue(value);
}

void medVtkViewItkDataImageInteractor::windowLevel(double &window, double &level)
{
    window = d->windowLevelParameter->value().at(0).toDouble();
    level = d->windowLevelParameter->value().at(1).toDouble();
}

void medVtkViewItkDataImageInteractor::moveToSlice(int slice)
{
    //TODO find a way to get woorldCoordinate for slice from vtkInria.
}

void medVtkViewItkDataImageInteractor::setWindow(double window)
{
    d->view2d->SetColorWindow(window, d->medVtkView->layer(d->imageData));
    d->view3d->SetColorWindow(window, d->medVtkView->layer(d->imageData));
}

void medVtkViewItkDataImageInteractor::setLevel(double level)
{
    d->view2d->SetColorLevel(level, d->medVtkView->layer(d->imageData));
    d->view3d->SetColorLevel(level, d->medVtkView->layer(d->imageData));
}

void medVtkViewItkDataImageInteractor::setWindowLevel(QList<QVariant> values)
{
    if(values.size() != 2 )
    {
        qWarning() << "Window/Level parameters are incorrect";
        return;
    }

    bool needUpdate = false;
    if (d->view2d->GetColorWindow(d->medVtkView->layer(d->imageData)) != values.at(0))
    {
        setWindow(values.at(0).toDouble());
        needUpdate = true;
    }
    if (d->view2d->GetColorLevel(d->medVtkView->layer(d->imageData)) != values.at(1))
    {
        setLevel(values.at(1).toDouble());
        needUpdate = true;
    }

    if(needUpdate)
        update();
}

void medVtkViewItkDataImageInteractor::update()
{
    if(d->medVtkView->is2D())
        d->view2d->Render();
    else
        d->view3d->Render();
}

void medVtkViewItkDataImageInteractor::updateWidgets()
{
    // slice orientation may differ from view orientation. Adapt slider range accordingly.
    int orientationId = d->view2d->GetSliceOrientation();
    if (orientationId==vtkImageView2D::SLICE_ORIENTATION_XY)
        d->slicingParameter->setRange(0, d->imageData->zDimension()-1);
    else if (orientationId==vtkImageView2D::SLICE_ORIENTATION_XZ)
        d->slicingParameter->setRange (0, d->imageData->yDimension()-1);
    else if (orientationId==vtkImageView2D::SLICE_ORIENTATION_YZ)
        d->slicingParameter->setRange (0, d->imageData->xDimension()-1);

    // update slider position
    if(d->medVtkView->is2D())
    {
        unsigned int zslice = d->view2d->GetSlice();
        d->slicingParameter->setValue(zslice);
    }
}

void medVtkViewItkDataImageInteractor::updateWindowLevelParam(double window, double level, unsigned int layer)
{
    if( d->medVtkView->layer(d->imageData) != layer )
        return;

    QList<QVariant> values;
    values.append(QVariant(window));
    values.append(QVariant(level));
    d->windowLevelParameter->setValue(values);
}

void medVtkViewItkDataImageInteractor::setUpViewForThumbnail()
{
    d->view2d->Reset();
    d->view2d->SetBackground(0.0, 0.0, 0.0);
    d->view2d->CursorFollowMouseOff();
    d->view2d->ShowImageAxisOff();
    d->view2d->ShowScalarBarOff();
    d->view2d->ShowAnnotationsOff();
    d->view2d->ShowRulerWidgetOff();
}


void medVtkViewItkDataImageInteractor::updateImageViewInternalLayer()
{
    if( d->medVtkView->layer(d->imageData) != d->medVtkView->currentLayer() )
        return;

    d->view2d->SetCurrentLayer(d->medVtkView->layer(d->imageData));
    d->view3d->SetCurrentLayer(d->medVtkView->layer(d->imageData));

}

