/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtGui>
#include <QtWidgets>

#include <medAbstractArea.h>

class medHomepageAreaPrivate;
class medHomepageArea : public medAbstractArea
{
Q_OBJECT
public:
    medHomepageArea(QWidget * parent = 0);
    virtual ~medHomepageArea();

    void initPage();
    QParallelAnimationGroup * getAnimation();
    void resizeEvent(QResizeEvent * event);

    virtual QString title() const {return "HomePage";}
    virtual QString description() const {return "Caca prout";}
    virtual QIcon icon()  const {return QIcon(":icons/home.png");}

public slots:
    void onShowBrowser();
    void onShowWorkspace(QString workspace);
    void onShowSettings();
    void onShowAbout();
    void onShowPlugin();
    void onShowHelp();
    void onShowInfo();

signals:
    /**
     * @deprecated Doesn't use the medMainWindow's slots anymore.
     */
    void showSettings();
    /**
     * @deprecated Doesn't use the medMainWindow's slots anymore.
     */
    void showAbout();
    /**
     * @deprecated Doesn't use the medMainWindow's slots anymore.
     */
    void showHelp();

    void showBrowser();
    void showViewer();
    void showWorkspace(QString workspace);

private:
    medHomepageAreaPrivate * d;
};

inline medAbstractArea* medHomepageAreaCreator(void)
{
    return new medHomepageArea();
}

