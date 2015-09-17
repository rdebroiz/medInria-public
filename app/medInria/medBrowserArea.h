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

class medBrowserAreaPrivate;
class medAbstractDataSource;
class medToolBox;
class medJobItem;

class medBrowserArea : public medAbstractArea
{
    Q_OBJECT

public:
     medBrowserArea(QWidget *parent = 0);
    ~medBrowserArea();

     virtual QString title() const {return "Browser";}
     virtual QString description() const {return "Pipi caca";}
     virtual QIcon icon() const {return QIcon(":icons/folder.png");}

public slots:
    void onSourceIndexChanged(int index);

    /**
* @brief Adds a medToolBox to the medToolBoxContainer.
*
* @param toolbox
*/
    void addToolBox(medToolBox *toolbox);
    /**
* @brief Removes a medToolBox from the medToolBoxContainer.
*
* @param toolbox
*/
    void removeToolBox(medToolBox *toolbox);

protected:
    void setToolBoxesVisible(int index, bool visible);
    void addDataSource(medAbstractDataSource* dataSource);

private:
    medBrowserAreaPrivate *d;
};

inline medAbstractArea* medBrowserAreaCreator(void)
{
    return new medBrowserArea();
}
