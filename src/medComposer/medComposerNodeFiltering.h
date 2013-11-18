/* medComposerNodeFiltering.h ---
 * 
 * Author: Thibaud Kloczko
 * Created: lun. nov. 18 14:00:54 2013 (+0100)
 * Version: 
 * Last-Updated: lun. nov. 18 14:12:48 2013 (+0100)
 *           By: Thibaud Kloczko
 *     Update #: 9
 */

/* Change Log:
 * 
 */

#pragma once

#include <dtkComposer/dtkComposerNodeLeafProcess.h>

class medComposerNodeFilteringPrivate;

// /////////////////////////////////////////////////////////////////
// numComposerNodeMeshBuilder interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeFiltering : public dtkComposerNodeLeafProcess
{
public:
     medComposerNodeFiltering(void);
    ~medComposerNodeFiltering(void);

public:
    QString type(void);
    QString titleHint(void);

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

public:
    bool isAbstractProcess(void) const;

    QString abstractProcessType(void) const;

public:
    void setProcess(dtkAbstractProcess *process);

    dtkAbstractProcess *process(void) const;

public:
    void run(void);

private:
    medComposerNodeFilteringPrivate *d;
};
