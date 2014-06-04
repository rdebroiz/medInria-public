#include "medComposerNodeImageWriter.h"

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>


#include <medAbstractDataFactory.h>
#include <dtkCore/dtkAbstractProcess.h>


#include <medAbstractImageData.h>

#include <dtkMath/dtkVector.h>

// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlenderPrivate declaration
// /////////////////////////////////////////////////////////////////

class medComposerNodeImageWriterPrivate
{

public:
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;
    dtkComposerTransmitterReceiver<QString> receiver_file;

public:
    dtkComposerTransmitterEmitter<dtkAbstractData> emitter_image;
};

// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlender implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeImageWriter::medComposerNodeImageWriter(void) : dtkComposerNodeLeafData(), d(new medComposerNodeImageWriterPrivate)
{
    this->appendReceiver(&(d->receiver_image));
    this->appendReceiver(&(d->receiver_file));

    this->appendEmitter(&(d->emitter_image));
}

medComposerNodeImageWriter::~medComposerNodeImageWriter(void)
{
    delete d;

    d = NULL;
}

bool medComposerNodeImageWriter::isAbstractData(void) const
{
    return false;
}

QString medComposerNodeImageWriter::abstractDataType(void) const
{
    return "";
}

void medComposerNodeImageWriter::run(void)
{
    bool written = false;
    QString filename = *d->receiver_file.data();
    dtkAbstractData* imData = d->receiver_image.data();
    if(!imData)
    {
        qWarning() << "attempt to write NULL data";
        return;
    }

    QList<QString> writers = medAbstractDataFactory::instance()->writers();

    if ( writers.size() ==0 )
    {
        return;
    }

    // cycle through readers to see if the last used reader can handle the file
    dtkSmartPointer<dtkAbstractDataWriter> dataWriter = NULL;
    dtkSmartPointer<dtkAbstractDataWriter> tempdataWriter = NULL;

    for (int i=0; i<writers.size(); i++)
    {
        tempdataWriter = medAbstractDataFactory::instance()->writerSmartPointer(writers[i]);
        if (tempdataWriter->handled().contains(imData->identifier()) && tempdataWriter->canWrite(filename))
        {
            tempdataWriter->enableDeferredDeletion(false);
            dataWriter = tempdataWriter;
            //break;
        }
    }

    if(dataWriter)
    {
        dtkDebug() << "Writing file";
        dataWriter->setData(imData);
        written = dataWriter->write(filename);
        if(written)
        {
            qDebug() << "Write success";
            d->emitter_image.setData(imData);
        }
        else
            qDebug() << "Write failure";
    }
}

QString medComposerNodeImageWriter::type(void)
{
    return "medDataImageWriter";
}

QString medComposerNodeImageWriter::titleHint(void)
{
    return "Writer";
}

QString medComposerNodeImageWriter::inputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    case 1:
        return "file";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeImageWriter::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}
