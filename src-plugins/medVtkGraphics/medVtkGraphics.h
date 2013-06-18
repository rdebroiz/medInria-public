/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkAbstractView.h>

#include <medAbstractView.h>
#include "medVtkGraphicsPluginExport.h"

class medVtkGraphicsPrivate;
class medAbstractViewCoordinates;
class v3dViewObserver;
class vtkImageView;
class vtkImageView2D;
class vtkImageView3D;
class vtkRenderer;
class vtkRenderWindowInteractor;

class MEDVTKGRAPHICSPLUGIN_EXPORT medVtkGraphics : public medAbstractView
{
    Q_OBJECT

public:
             medVtkGraphics();
    virtual ~medVtkGraphics();

    virtual QString description() const;
    virtual QString identifier() const;

    static bool registered();
    static QString s_identifier();

public:
    // inherited from medAbstractView
    void reset();
    void clear();
    void update();

    void *view();

    void setData(dtkAbstractData *data, int layer);

    void setSharedDataPointer(dtkSmartPointer<dtkAbstractData> data);

    void setData(dtkAbstractData *data);

    void *data();

    QWidget *receiverWidget();
    QWidget *widget();

    void close();

    /**
     * Returns pointers to internal instances of vtkImageView2D/3D.
     */
    vtkImageView2D *view2d();
    vtkImageView3D *view3d();
    
    /**
     * Returns a pointer to the current view, being 2D or 3D.
     */
    vtkImageView *currentView();

    /**
     * Returns the vtkRenderWindowInteractor instance.
     */
    vtkRenderWindowInteractor *interactor();

    /**
     * Access methods to vtkRenderer members.
     */
    vtkRenderer *renderer2d();
    vtkRenderer *renderer3d();
    
    medAbstractViewCoordinates * coordinates();

public slots:
    // inherited from medAbstractView
    void onPositionChanged  (const  QVector3D &position);
    void onZoomChanged      (double zoom);
    void onPanChanged       (const  QVector2D &pan);
    void onWindowingChanged (double level, double window);
    void onCameraChanged    (const  QVector3D &position,
                             const  QVector3D &viewup,
                             const  QVector3D &focal,
                             double parallelScale);

    void onVisibilityChanged(bool   visible, int layer);
    void onOpacityChanged   (double opacity, int layer);

private:
     medVtkGraphicsPrivate *d;

     template <typename IMAGE>
     bool SetViewInput(const char* type,dtkAbstractData* data,const int layer);
     bool SetView(const char* type,dtkAbstractData* data);
     template <typename IMAGE>
     bool SetViewInputWithConversion(const char* type,const char* newtype,dtkAbstractData* data,const int layer);
     
     friend class v3dViewObserver;
};

dtkAbstractView *createmedVtkGraphics();


