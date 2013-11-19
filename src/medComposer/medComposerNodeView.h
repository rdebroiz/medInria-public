/* medComposerNodeView.h ---
 *
 * Author:
 * Created: Mon Nov 18 12:17:37 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 18
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkComposer/dtkComposerNodeLeafView.h>

class medComposerNodeViewPrivate;

class medComposerNodeView : public dtkComposerNodeLeafView
{
public:
     medComposerNodeView(void);
    ~medComposerNodeView(void);

public:
    bool isAbstractView(void) const;

    QString abstractViewType(void) const;

public:
    void run(void);

public:
    QString type(void);
    QString titleHint(void);

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

public:
    QGraphicsWidget *widget(QGLContext *context);

 private:
    medComposerNodeViewPrivate *d;
};
