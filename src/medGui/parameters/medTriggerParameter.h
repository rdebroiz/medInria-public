/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medAbstractParameter.h>

#include <medGuiExport.h>

class QIcon;
class QPushButton;
class QSize;
class QWidget;

class medTriggerParameterPrivate;
class MEDGUI_EXPORT medTriggerParameter : public medAbstractTriggerParameter
{
    Q_OBJECT

public:
    medTriggerParameter(QString name = "Unknow trigger parameter", QObject* parent = 0);
    virtual ~medTriggerParameter();

    void setButtonIcon(QIcon& icon);
    void setButtonIconSize(QSize& size);
    void setButtonText(QString&  text);
    QPushButton* getPushButton();

    virtual QWidget* getWidget();
private slots:
    void _prvt_removeInternPushButton();

private:
    medTriggerParameterPrivate* d;
};
