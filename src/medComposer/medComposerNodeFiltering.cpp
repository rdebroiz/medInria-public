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

#include <dtkCore/dtkAbstractProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>


// /////////////////////////////////////////////////////////////////
// medComposerNodeFilteringPrivate interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeFilteringPrivate
{
public:
    dtkAbstractProcess *filtering;

public:
    dtkComposerTransmitterReceiver<dtkAbstractData> receiver_data;

public:
    dtkComposerTransmitterEmitter<dtkAbstractData> emitter_data;

public:
    qlonglong index;
    qreal value;
};

// /////////////////////////////////////////////////////////////////
// medComposerNodeFiltering implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeFiltering::medComposerNodeFiltering(void) : dtkComposerNodeLeafProcess(), d(new medComposerNodeFilteringPrivate)
{
    this->appendReceiver(&(d->receiver_data));

    this->appendEmitter(&(d->emitter_data));

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
        return "data";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeFiltering::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "data";
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
    d->filtering = process;
}

dtkAbstractProcess *medComposerNodeFiltering::process(void) const
{
    return d->filtering;
}

void medComposerNodeFiltering::run()
{
    if (!d->receiver_data.isEmpty()) {

        if (!d->filtering){
            dtkWarn() << Q_FUNC_INFO << "No process instantiated, abort:" << this->currentImplementation();
            d->emitter_data.clearData();
            return;
        }

        dtkAbstractData *data = d->receiver_data.data();

        if (!data) {
            dtkError() << Q_FUNC_INFO << "Input data is not allocated";
            return;
        }


        d->filtering->setInput(data);

        d->index = d->filtering->run();

        d->emitter_data.setData(d->filtering->output());

    } else {

        dtkWarn() << Q_FUNC_INFO << "The input are not all set. Nothing is done.";
        d->emitter_data.clearData();
    }
}
