/* medComposerNodeFiltering.cpp ---
 * 
 * Author: Thibaud Kloczko
 * Created: lun. nov. 18 14:03:59 2013 (+0100)
 * Version: 
 * Last-Updated: lun. nov. 18 14:28:08 2013 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 25
 */

/* Change Log:
 * 
 */

#include "medComposerNodeFiltering.h"

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <medAbstractFilteringProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

#include <medAbstractImageData.h>

// /////////////////////////////////////////////////////////////////
// medComposerNodeFilteringPrivate interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeFilteringPrivate
{
public:
    medAbstractFilteringProcess *filtering;

public:
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;

public:
    dtkComposerTransmitterEmitter<medAbstractImageData> emitter_image;

public:
    qlonglong index;
    qreal value;
};

// /////////////////////////////////////////////////////////////////
// medComposerNodeFiltering implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeFiltering::medComposerNodeFiltering(void) : dtkComposerNodeLeafProcess(), d(new medComposerNodeFilteringPrivate)
{
    this->appendReceiver(&(d->receiver_image));

    this->appendEmitter(&(d->emitter_image));

    d->filtering = NULL;
}

medComposerNodeFiltering::~medComposerNodeFiltering(void)
{
    this->clearProcess();

    delete d;
    d = NULL;
}

QString medComposerNodeFiltering::type(void) 
{
    return "medFiltering";
}

QString medComposerNodeFiltering::titleHint(void)
{
    return "Filtering";
}

QString medComposerNodeFiltering::inputLabelHint(int port) 
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeFiltering::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}

bool medComposerNodeFiltering::isAbstractProcess(void) const
{
    return true;
}

QString medComposerNodeFiltering::abstractProcessType(void) const
{
    return "dtkAbstractProcess";
}

void medComposerNodeFiltering::setProcess(dtkAbstractProcess *process)
{
    d->filtering = dynamic_cast<medAbstractFilteringProcess*>(process);
}

dtkAbstractProcess *medComposerNodeFiltering::process(void) const
{
    return d->filtering;
}

void medComposerNodeFiltering::run()
{
    if (!d->receiver_image.isEmpty()) {

        if (!d->filtering){
            dtkWarn() << Q_FUNC_INFO << "No process instantiated, abort:" << this->currentImplementation();
            d->emitter_image.clearData();
            return;
        }

        medAbstractImageData *image = qobject_cast<medAbstractImageData *>(d->receiver_image.data());

        if (!image) {
            dtkError() << Q_FUNC_INFO << "Input image is not allocated";
            return;
        }


        d->filtering->setInputImage(image);

        d->index = d->filtering->run();

        d->emitter_image.setData(qobject_cast<medAbstractImageData *>(d->filtering->output()));

    } else {

        dtkWarn() << Q_FUNC_INFO << "The input are not all set. Nothing is done.";
        d->emitter_image.clearData();
    }
}
