/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medToolBox.h>
#include <medGuiExport.h>

class dtkAbstractView;
class medAbstractData;
class dtkAbstractProcess;
class medDiffusionSelectorToolBoxPrivate;
class medDataIndex;

class MEDGUI_EXPORT medDiffusionSelectorToolBox : public medToolBox
{
    Q_OBJECT
public:
     medDiffusionSelectorToolBox(QWidget *parent = 0);
    ~medDiffusionSelectorToolBox();

    medAbstractData *output() const;

signals:
    void newOutput(medAbstractData * data);

public slots:
    // void run();
    void clear();

    void onToolBoxChosen(int id);

private:
    medDiffusionSelectorToolBoxPrivate *d;

};


