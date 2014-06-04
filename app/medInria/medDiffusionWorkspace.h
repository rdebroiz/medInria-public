/*=========================================================================

medInria

Copyright (c) INRIA 2013. All rights reserved.
See LICENSE.txt for details.
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.

=========================================================================*/

#pragma once


#include <QtCore>

#include <medAbstractWorkspace.h>
#include <medDiffusionSelectorToolBox.h>

class medAbstractData;
class medDiffusionWorkspacePrivate;

class medDiffusionWorkspace : public medAbstractWorkspace
{
    Q_OBJECT

public:
     medDiffusionWorkspace(QWidget *parent = 0);
    ~medDiffusionWorkspace();

    virtual QString identifier() const;
    virtual QString description() const;
    static bool isUsable();
    void setupViewContainerStack();
    
public slots:

    void runProcess(QString processName, QString category);
    void getOutput();
    void cancelProcess();
    void resetRunningFlags();
    
    //Handle new data in central view, connect them to toolboxes
    void resetToolBoxesInputs();
    void updateToolBoxesInputs();
    void addToolBoxInput(medAbstractData *data);

    void changeCurrentContainer();

private:
    medDiffusionWorkspacePrivate *d;
};
