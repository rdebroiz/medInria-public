/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractImageViewInteractor.h>

#include <itkDataImagePluginExport.h>

class medVtkViewItkVectorFieldInteractorPrivate;

class ITKDATAIMAGEPLUGIN_EXPORT medVtkViewItkVectorFieldInteractor : public medAbstractImageViewInteractor
{
    Q_OBJECT

public:
    medVtkViewItkVectorFieldInteractor(medAbstractView* parent);
    ~medVtkViewItkVectorFieldInteractor();

    virtual QString description() const;
    virtual QString identifier() const;
    virtual QStringList handled() const;

    static bool registered();

    virtual void setData(medAbstractData * data);

    virtual void windowLevel(double &window, double &level);

    double opacity() const;
    bool visibility() const;

    virtual QWidget* buildLayerWidget();
    virtual QWidget* buildToolBoxWidget();
    virtual QWidget* buildToolBarWidget();

    virtual QList<medAbstractParameter*> parameters();

    virtual void removeData();

public slots:

    void setScale(double scale);
    void setSampleRate(int sampleRate);
    void setColorMode(QString mode);
    void setProjection(bool enable);
    void setShowAxial(bool show);
    void setShowCoronal(bool show);
    void setShowSagittal(bool show);
    virtual void moveToSlice  (int slice);

    /** Change the position of the slices */
    void setPosition(const QVector3D& position);

    void setOpacity(double opacity);
    void setVisibility(bool visibility);
    void setWindowLevel(double &window, double &level);

    virtual void setUpViewForThumbnail();

    virtual void updateWidgets();


protected:
    void setupParameters();

private:
    static QStringList dataHandled();
    void update();

private slots:
    void updateSlicingParam();


private:
    medVtkViewItkVectorFieldInteractorPrivate* d;
};