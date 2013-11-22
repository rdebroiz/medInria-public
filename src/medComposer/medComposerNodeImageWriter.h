#ifndef medComposerNodeImageWriter_H
#define medComposerNodeImageWriter_H

#include <dtkComposer/dtkComposerNodeLeafData.h>

#include <QtCore>

class medComposerNodeImageWriterPrivate;

class medComposerNodeImageWriter : public dtkComposerNodeLeafData
{
public:
     medComposerNodeImageWriter(void);
    ~medComposerNodeImageWriter(void);

public:
    bool isAbstractData(void) const;

    QString abstractDataType(void) const;

public:
    void run(void);

public:
    QString type(void);
    QString titleHint(void) ;

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

private:
    medComposerNodeImageWriterPrivate *d;
};

#endif
