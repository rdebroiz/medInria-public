/* medComposerNodeView.cpp ---
 *
 * Author:
 * Created: Mon Nov 18 12:17:52 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 28
 */

/* Change Log:
 *
 */

#include "medComposerNodeView.h"

#include <medCore/medAbstractDataImage.h>
#include <medCore/medAbstractView.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractView.h>

// /////////////////////////////////////////////////////////////////
// medComposerNodeViewPrivate declaration
// /////////////////////////////////////////////////////////////////

class medComposerNodeViewPrivate
{
public:
    medAbstractView *view;

public:
    dtkComposerTransmitterReceiver<medAbstractDataImage> receiver_image;
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
    foreach(medAbstractDataImage *image, d->receiver_image.allData())
        d->view->setData(image);

    d->view->widget()->show();
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
