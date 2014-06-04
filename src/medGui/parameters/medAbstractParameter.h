/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QObject>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

#include <medGuiExport.h>

class QLabel;

class medAbstractParameterPrivate;
class MEDGUI_EXPORT medAbstractParameter : public QObject
{
    Q_OBJECT

public:

    medAbstractParameter(QString name = "unknown parameter", QObject* parent = 0);
    virtual ~medAbstractParameter();

    QString name() const;
    void setName(QString &name);
    QLabel* getLabel();

    /**
     * @brief getWidget Return a default widget.
     * @return QWidget*
     */
    virtual QWidget* getWidget() = 0;

    bool match(medAbstractParameter const *other);
    void show();
    void hide();

    void setToolTip(QString& tooltip);

protected:
    void addToInternWidgets(QWidget * widget);
    void blockInternWidgetsSignals(bool block) const;

protected slots:
    void removeFromInternWidgets(QWidget* widget);

private slots:
    void removeInternLabel();

signals:
    void aboutToBeDestroyed();

private:
  medAbstractParameterPrivate *d; 
};



//--------------------------------------------------------------------------
//  medTestParameter

class medTestParameterPrivate;
/**
 * @brief The medTestParameter class is just a dummy implementaion of
 * medAbstractParameter to test its interface.
 */
class MEDGUI_EXPORT medTestParameter : public medAbstractParameter
{
    Q_OBJECT

public:
    medTestParameter(QString name = "unknown parameter");
    virtual~medTestParameter();

    /**
     * @brief getWidget Return a default widget.
     * @return QWidget*
     */
    virtual QWidget* getWidget();

private slots:
    void removeInternWidget();

private:
  medTestParameterPrivate *d;
};

//--------------------------------------------------------------------------
//  medAbstractTriggerParameter
class MEDGUI_EXPORT medAbstractTriggerParameter : public medAbstractParameter
{
    Q_OBJECT
public:
    medAbstractTriggerParameter(QString name = "unknown trigger parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
public slots:
    virtual void trigger();

signals:
    void triggered();

};
//--------------------------------------------------------------------------
//  medAbstractStringParameter
class MEDGUI_EXPORT medAbstractStringParameter : public medAbstractParameter
{
    Q_OBJECT
public:
    medAbstractStringParameter(QString name = "unknown  string parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
    virtual QString value() const;
public slots:
    virtual void setValue(QString value);
signals:
    void valueChanged(QString);
protected:
    /**
     * @brief updateInternWigets
     * this methode is called by setValue(QString value) update the intern widget
     * of your medParameter implementaion
     */
    virtual void updateInternWigets() = 0;
protected:
    QString m_value;
};
//--------------------------------------------------------------------------
//  medAbstractIntParameter
class MEDGUI_EXPORT medAbstractIntParameter : public medAbstractParameter
{
    Q_OBJECT
public:
    medAbstractIntParameter(QString name = "unknown int parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
    virtual int value() const;
public slots:
    virtual void setValue(int value);
protected:
    /**
     * @brief updateInternWigets
     * this methode is called by setValue(int value) update the intern widget
     * of your medParameter implementaion
     */
    virtual void updateInternWigets() = 0;
signals:
    void valueChanged(int);

protected:
    int m_value;
};
//--------------------------------------------------------------------------
//  medAbstractDoubleParameter
class MEDGUI_EXPORT medAbstractDoubleParameter : public medAbstractParameter
{
    Q_OBJECT
public:
    medAbstractDoubleParameter(QString name = "unknown double parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
    virtual double value() const;
public slots:
    virtual void setValue(double value);
protected:
    /**
     * @brief updateInternWigets
     * this methode is called by setValue(double value) update the intern widget
     * of your medParameter implementaion
     */
    virtual void updateInternWigets() = 0;
signals:
    void valueChanged(double);
protected:
    double m_value;
};
//--------------------------------------------------------------------------
//  medAbstractBoolParameter
class MEDGUI_EXPORT medAbstractBoolParameter : public medAbstractParameter
{
    Q_OBJECT
public:
    medAbstractBoolParameter(QString name = "unknown bool parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
    virtual bool value() const;
public slots:
    virtual void setValue(bool value);
protected:
    /**
     * @brief updateInternWigets
     * this methode is called by setValue(bool value) update the intern widget
     * of your medParameter implementaion
     */
    virtual void updateInternWigets() = 0;

signals:
    void valueChanged(bool);

protected:
    bool m_value;
};

//--------------------------------------------------------------------------
//  medAbstractGroupParameter
class MEDGUI_EXPORT medAbstractGroupParameter : public medAbstractParameter
{
    Q_OBJECT
public:
    medAbstractGroupParameter(QString name = "unknown group parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}

    /**
     * @brief parametersCandidateToPool reimplement this methode to keep correct synchronisation in medParameterPool
     * @return A list of parameters that have to be linked when you had a
     * medAbstractGroupParameter to a medParameterPool.
     */
    virtual QList<medAbstractParameter*> parametersCandidateToPool() const = 0;
};




//--------------------------------------------------------------------------
//  medAbstractVector2DParameter

class QVector2D;

class MEDGUI_EXPORT medAbstractVector2DParameter : public medAbstractParameter
{
    Q_OBJECT

public:
    medAbstractVector2DParameter(QString name = "Unknow Vector 2D parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
    virtual ~medAbstractVector2DParameter(){}

    virtual QVector2D value() const;

public slots:
    virtual void setValue(const QVector2D &);

protected:
    virtual void updateInternWigets() = 0;

signals:
    void valueChanged(const QVector2D & );


protected:
    QVector2D m_value;

};

//--------------------------------------------------------------------------
//  medAbstractVector3DParameter

class QVector3D;

class MEDGUI_EXPORT medAbstractVector3DParameter : public medAbstractParameter
{
    Q_OBJECT

public:
    medAbstractVector3DParameter(QString name = "Unknow Vector 3D parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
    virtual ~medAbstractVector3DParameter(){}

    virtual QVector3D value() const;

public slots:
    virtual void setValue(const QVector3D &);

protected:
    virtual void updateInternWigets() = 0;

signals:
    void valueChanged(const QVector3D & );


protected:
    QVector3D m_value;

};



//--------------------------------------------------------------------------
//  medAbstractVector4DParameter

class QVector4D;

class MEDGUI_EXPORT medAbstractVector4DParameter : public medAbstractParameter
{
    Q_OBJECT

public:
    medAbstractVector4DParameter(QString name = "Unknow Vector 3D parameter", QObject* parent = 0):
        medAbstractParameter(name, parent){}
    virtual ~medAbstractVector4DParameter(){}

    virtual QVector4D value() const;

public slots:
    virtual void setValue(const QVector4D &);

protected:
    virtual void updateInternWigets() = 0;

signals:
    void valueChanged(const QVector4D & );


protected:
    QVector4D m_value;

};
