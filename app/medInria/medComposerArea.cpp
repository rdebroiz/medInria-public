/* medComposerArea.cpp ---
 *
 * Author:
 * Created: Mon Nov 18 10:58:54 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 9
 */

/* Change Log:
 *
 */

#include "medComposerArea.h"

class medComposerAreaPrivate
{
public:

};

medComposerArea::medComposerArea(QWidget *parent) : QWidget(parent)
{
    d = new medComposerAreaPrivate;
}

medComposerArea::~medComposerArea(void)
{
    delete d;
}

void medComposerArea::setup(QStatusBar *status)
{
    Q_UNUSED(status);
}

void medComposerArea::setdw(QStatusBar *status)
{
    Q_UNUSED(status);
}
