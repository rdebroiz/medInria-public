/* medComposerNodeView.cpp ---
 *
 * Author:
 * Created: Mon Nov 18 12:17:52 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 46
 */

/* Change Log:
 *
 */

#include "medComposerNodeView.h"

#include <medAbstractImageData.h>
#include <medAbstractView.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractView.h>

#include <medAbstractLayeredView.h>

// /////////////////////////////////////////////////////////////////
// medComposerNodeViewPrivate declaration
// /////////////////////////////////////////////////////////////////

class medComposerNodeViewPrivate
{
public:
    medAbstractView *view;

public:
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;
};

// /////////////////////////////////////////////////////////////////
// medComposerNodeView implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeView::medComposerNodeView(void) : dtkComposerNodeLeafView(), d(new medComposerNodeViewPrivate)
{
    d->view = static_cast<medAbstractView *>(dtkAbstractViewFactory::instance()->create("v3dView"));

    this->appendReceiver(&(d->receiver_image));
}

medComposerNodeView::~medComposerNodeView(void)
{
    delete d;

    d = NULL;
}

bool medComposerNodeView::isAbstractView(void) const
{
    return false;
}

QString medComposerNodeView::abstractViewType(void) const
{
    return "medAbstractView";
}

void medComposerNodeView::run(void)
{
    foreach(medAbstractImageData *image, d->receiver_image.allData())
    {
        medAbstractLayeredView *layerdView = dynamic_cast<medAbstractLayeredView *>(d->view);
        if(layerdView)
            layerdView->addLayer(image);
    }

}

QString medComposerNodeView::type(void)
{
    return "medAbstractView";
}

QString medComposerNodeView::titleHint(void)
{
    return "View";
}

QString medComposerNodeView::inputLabelHint(int port)
{
    switch(port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeView::outputLabelHint(int port)
{
    return dtkComposerNodeLeaf::outputLabelHint(port);
}

QGraphicsWidget *medComposerNodeView::widget(QGLContext *context)
{
    qDebug() << Q_FUNC_INFO << "Requesting decoration";
    return NULL;
    // TODO graphicsWidget for view ? ;) - RDE
//    return d->view->item(context);
}
