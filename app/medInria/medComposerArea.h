/* medComposerArea.h ---
 *
 * Author:
 * Created: Mon Nov 18 10:57:24 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 10
 */

/* Change Log:
 *
 */

#pragma once

#include <QtCore>
#include <QtGui>

class medComposerArea : public QWidget
{
    Q_OBJECT

public:
     medComposerArea(QWidget *parent = 0);
    ~medComposerArea(void);

    void setup(QStatusBar *status);
    void setdw(QStatusBar *status);

private:
    class medComposerAreaPrivate *d;
};
