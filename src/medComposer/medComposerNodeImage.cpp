/* medComposerNodeImage.cpp ---
 *
 * Author: Nicolas Niclausse
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "medComposerNodeImage.h"

#include <medAbstractImageData.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <medAbstractDataFactory.h>
//#include <dtkCore/dtkAbstractProcess.h>



// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlenderPrivate declaration
// /////////////////////////////////////////////////////////////////

class medComposerNodeImagePrivate
{
public:
    dtkSmartPointer<dtkAbstractDataReader> dataReader;

public:
    dtkComposerTransmitterReceiver<QString> receiver_filename;
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;

public:
    dtkComposerTransmitterEmitter<medAbstractImageData> emitter_image;
};

// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlender implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeImage::medComposerNodeImage(void) : dtkComposerNodeLeafData(), d(new medComposerNodeImagePrivate)
{
    d->dataReader = NULL;

    this->appendReceiver(&(d->receiver_filename));
    this->appendReceiver(&(d->receiver_image));

    this->appendEmitter(&(d->emitter_image));
}

medComposerNodeImage::~medComposerNodeImage(void)
{
    delete d;

    d = NULL;
}

bool medComposerNodeImage::isAbstractData(void) const
{
    return false;
}

QString medComposerNodeImage::abstractDataType(void) const
{
    return "medAbstractImageData";
}

void medComposerNodeImage::run(void)
{
    QString filename;

    if(!d->receiver_filename.isEmpty()) {
        filename = *(d->receiver_filename.data());
    }

    if (!d->receiver_filename.isEmpty() && !(filename.isEmpty())) {

        bool read = false;

        QList<QString> readers = medAbstractDataFactory::instance()->readers();

        if ( readers.size() == 0 ) {
            qDebug() <<  "No image readers found";
            return;
        }

        // cycle through readers to see if the last used reader can handle the file
        dtkSmartPointer<dtkAbstractDataReader> tempdataReader = NULL;

        for (int i=0; i<readers.size(); i++) {
            tempdataReader = medAbstractDataFactory::instance()->readerSmartPointer(readers[i]);
            if (tempdataReader->canRead(filename)) {
                /*d->lastSuccessfulReaderDescription = dataReader->identifier();*/
                tempdataReader->enableDeferredDeletion(false);
                d->dataReader = tempdataReader;
                break;
            }
        }

        if(d->dataReader)
        {
            read = d->dataReader->read(filename);
            d->emitter_image.setData(dynamic_cast<medAbstractImageData *>(d->dataReader->data()));

            if(read)
                qDebug() << "Read success";
            else
                qDebug() << "Read failure";
        } else {
            qDebug() << "no reader !";
        }

    } else  if (!d->receiver_image.isEmpty()) {
        medAbstractImageData *image = d->receiver_image.data();
        d->emitter_image.setData(image);
    } else {
        qDebug() << Q_FUNC_INFO << " No port connected";
    }

}

QString medComposerNodeImage::type(void)
{
    return "medAbstractImageData";
}

QString medComposerNodeImage::titleHint(void)
{
    return "ImageReader";
}

QString medComposerNodeImage::inputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "file";
    case 1:
        return "image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeImage::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}
