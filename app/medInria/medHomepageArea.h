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

class medHomepageArea : public medAbstractArea
{
Q_OBJECT
public:
    medHomepageArea(QObject * parent = NULL);
    virtual ~medHomepageArea();

    void initPage();

    virtual QString title() const {return "HomePage";}
    virtual QString description() const {return "Caca prout";}
    virtual QIcon icon()  const {return QIcon(":icons/home.png");}
    virtual QPixmap altTabPixmap()  const {return QPixmap(":icons/home.png");}
    virtual QWidget* centralWidget()  const;
    virtual QWidget* statusBarWidget()  const;

public slots:
    void showHelp() const;

};

class medHomePageCentralWidget: public QWidget
{
    Q_OBJECT
public:
    medHomePageCentralWidget(QWidget *parent = NULL);
    virtual ~medHomePageCentralWidget();

signals:
    void showHelpRequest();
};

inline medAbstractArea* medHomepageAreaCreator(void)
{
    return new medHomepageArea();
}

