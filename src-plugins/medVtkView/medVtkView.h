#pragma once

#include <medAbstractImageView.h>

class medVtkViewPrivate;
class medAbstractData;
class medAbstractImageData;


class medVtkView : public medAbstractImageView
{
    Q_OBJECT
    
public:

    medVtkView(QObject* parent = 0);
    virtual ~medVtkView();

    virtual QString  identifier() const;
    static QString  s_identifier();
    
    static bool registered();

    virtual medViewBackend * backend() const;

    virtual QString description() const;

    virtual QWidget *viewWidget();
    virtual QWidget *widget();

    virtual QWidget* toolBar();
    virtual QWidget* toolBox();

    virtual QList<medAbstractParameter*> viewParameters();

    virtual QPointF mapWorldToDisplayCoordinates(const QVector3D & worldVec );
    virtual QVector3D mapDisplayToWorldCoordinates(const QPointF & scenePoint );
    virtual QVector3D viewCenter();
    virtual QVector3D viewPlaneNormal();
    virtual QVector3D viewUp();
    virtual bool is2D();
    virtual qreal sliceThickness();
    virtual qreal scale();

public slots:
    virtual void reset();

private slots:
    void _prvt_setWindowingInteractionStyle(bool windowing);
    void _prvt_setZoomInteractionStyle(bool zoom);
    void _prvt_setSLicingInteractionStyle(bool slicing);

    void _prvt_buildToolBox();
    void _prvt_buildMouseInteracToolBox();

    void _prvt_addLayerItem(int layer);
    void _prvt_removeLayerItem(int layer);

    void _prvt_removeSelectedLayer();

    void _prvt_removeLayerData(int layer);

    void updateInteractorsWidget();

private:
    medVtkViewPrivate *d;
};

