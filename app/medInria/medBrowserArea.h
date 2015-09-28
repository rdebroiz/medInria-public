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

class medBrowserArea : public medAbstractArea
{
    Q_OBJECT

public:
     medBrowserArea(QObject *parent = NULL);
    ~medBrowserArea();

     virtual QString title() const {return "Browser";}
     virtual QString description() const {return "Pipi caca";}
     virtual QIcon icon() const {return QIcon(":icons/folder.png");}
     virtual QPixmap altTabPixmap()  const {return QPixmap(":icons/folder.png");}
     virtual QWidget* centralWidget()  const;
     virtual QWidget* statusBarWidget()  const;
};

inline medAbstractArea* medBrowserAreaCreator(void)
{
    return new medBrowserArea();
}

class medAbstractDatasource;
class medToolBox;

class medBrowserAreaCentralWidgetPrivate;
class medBrowserAreaCentralWidget: public QWidget
{
    Q_OBJECT
public:
    medBrowserAreaCentralWidget(QWidget *parent = NULL);
   ~medBrowserAreaCentralWidget();

protected:
    void _changeSource(int index);
    void _addDatasource(medAbstractDatasource* dataSource);
    void _setToolBoxesVisible(int index, bool visible);

private:
    medBrowserAreaCentralWidgetPrivate *d;

};
