// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef V3DVIEW_H
#define V3DVIEW_H

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractView.h>
#include <medCore/medAbstractView.h>

#include "v3dViewPluginExport.h"

class QMouseEvent;

class v3dViewPrivate;
class vtkImageView;
class vtkImageView2D;
class vtkImageView3D;
class vtkRenderer;
class vtkRenderWindowInteractor;

class V3DVIEWPLUGIN_EXPORT v3dView : public medAbstractView
{
    Q_OBJECT

public:
             v3dView(void);
    virtual ~v3dView(void);

    virtual QString description(void) const;

    static bool registered(void);

signals:
    void closed(void);
    void becomeDaddy (bool);
    void becameDaddy (bool);
    void sync (bool);
    void syncWL (bool);
    void reg(bool);

public:
    void reset(void);
    void clear(void);
    void update(void);

    void   link(dtkAbstractView *other);
    void unlink(dtkAbstractView *other);

    void *view(void);

    void setData(dtkAbstractData *data);
    void *data (void);

    QSet<dtkAbstractView *> linkedViews (void);

    QWidget *receiverWidget(void);
    QWidget *widget(void);

    // access method to internal members for v3dView**Interactor classes
    vtkImageView2D *viewAxial(void);
    vtkImageView2D *viewCoronal(void);
    vtkImageView2D *viewSagittal(void);
    vtkImageView3D *view3D(void);
    
    vtkRenderWindowInteractor *interactor(void);
    
    vtkRenderer *rendererAxial(void);
    vtkRenderer *rendererCoronal(void);
    vtkRenderer *rendererSagittal(void);
    vtkRenderer *renderer3D(void);

    void setColorLookupTable(QList<double>scalars,QList<QColor>colors);

public slots:
    void play   (bool);
    void linkwl (dtkAbstractView* view, bool);
    
public slots:
    void onPropertySet         (QString key, QString value);
    void onOrientationPropertySet           (QString value);
    void onModePropertySet                  (QString value);
    void onVRModePropertySet                (QString value);
    void onUseLODPropertySet                (QString value);
    void onPresetPropertySet                (QString value);
    void onScalarBarVisibilityPropertySet   (QString value);
    void onLookupTablePropertySet           (QString value);
    void onBackgroundLookupTablePropertySet (QString value);
    void onOpacityPropertySet               (QString value);
    void onShowAxisPropertySet              (QString value);
    void onShowRulerPropertySet             (QString value);
    void onShowAnnotationsPropertySet       (QString value);
    void onLeftClickInteractionPropertySet  (QString value);
    void onCroppingPropertySet              (QString value);
    void onMousePressEvent                  (QMouseEvent *event);
    void onZSliderValueChanged              (int value);
    void onDaddyPropertySet                 (QString value);
    void onLinkedWLPropertySet              (QString value);
	void onDimensionBoxChanged              (QString value);

    void onMetaDataSet(QString key, QString value);

public slots: // Menu interface
    void onMenuAxialTriggered               (void);
    void onMenuSagittalTriggered            (void);
    void onMenuCoronalTriggered             (void);
    void onMenu3DVRTriggered                (void);
    void onMenu3DMaxIPTriggered             (void);
    void onMenu3DMinIPTriggered             (void);
    void onMenu3DMPRTriggered               (void);
    void onMenu3DOffTriggered               (void);
    void onMenuVRGPUTriggered               (void);
    void onMenuVRRayCastAndTextureTriggered (void);
    void onMenuVRRayCastTriggered           (void);
    void onMenuVRTextureTriggered           (void);
    void onMenuVRDefaultTriggered           (void);
    void onMenu3DLODTriggered               (void);
    void onMenuZoomTriggered                (void);
    void onMenuWindowLevelTriggered         (void);

public:
    void  enableInteraction(void);
    void disableInteraction(void);
    void bounds(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax);
    void cameraUp(double *coordinates);
    void cameraPosition(double *coordinates);
    void cameraFocalPoint(double *coordinates);
    void setCameraPosition(double x, double y, double z);
    void setCameraClippingRange(double nearRange, double farRange);

    QString cameraProjectionMode(void);
    double cameraViewAngle(void);
    double cameraZoom(void);

private:
    v3dViewPrivate *d;
};

dtkAbstractView *createV3dView(void);

#endif
