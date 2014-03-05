/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QFrame>

#include <medGuiExport.h>


struct QUuid;
class medAbstractView;
class medAbstractData;
class medDataIndex;
class medToolBox;
class medViewContainerSplitter;

class medViewContainerPrivate;
class MEDGUI_EXPORT medViewContainer: public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool selected READ isSelected)

public:
    medViewContainer(medViewContainerSplitter* parent = 0);
    ~medViewContainer();


    bool isSelected() const;
    bool isMultiSelected() const;
    bool isMaximised() const;
    bool isUserSplittable() const;
    bool isUserClosable() const;
    bool isMultiLayered() const;
    medAbstractView* view() const;
    QUuid uuid() const;
    void setContainerParent(medViewContainerSplitter* splitter);
    medViewContainer* vSplit();
    medViewContainer* hSplit();
    medViewContainer* split(Qt::AlignmentFlag alignement = Qt::AlignRight);

public slots:
    void setSelected(bool selected);
    void setUnSelected(bool unSelected);
    void setMaximised(bool maximised);
    void setUserSplittable(bool splittable);
    void setUserClosable(bool closable);
    void setMultiLayered(bool multiLayer);
    void setView(medAbstractView* view);
    void addData(medAbstractData* data);
    void highlight(QString color = "#FFBB77");
    void unHighlight();
    void link(QString pool);
    void unlink();

signals:
    void maximised(QUuid uuid, bool maximised);
    void maximised(bool maximised);
    void containerSelected(QUuid uuid);
    void containerUnSelected(QUuid uuid);
    void currentLayerChanged();
    void vSplitRequest();
    void hSplitRequest();
    void splitRequest(medDataIndex, Qt::AlignmentFlag);
    void viewChanged();
    void viewRemoved();
    void linkRequested(QUuid uuid, QString);
    void unlinkRequested(QUuid uuid);


protected:
    void focusInEvent(QFocusEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    void recomputeStyleSheet();

protected slots:
    void removeView();
    void emitLinkRequested(QString pool);

private slots:
    void removeInterneView();
    void selfDestroy();
    void createDragLabels();
    void destroyDragLabels();


private:
    medViewContainerPrivate *d;

};
