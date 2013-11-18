/* medComposerArea.h ---
 *
 * Author:
 * Created: Mon Nov 18 10:57:24 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 14
 */

/* Change Log:
 *
 */

#pragma once

#include <QtCore>
#include <QtGui>

#include <dtkComposer/dtkComposerWriter.h>

class medComposerArea : public QWidget
{
    Q_OBJECT

public:
     medComposerArea(QWidget *parent = 0);
    ~medComposerArea(void);

    void setup(QStatusBar *status);
    void setdw(QStatusBar *status);

public slots:
    bool compositionOpen(void);
    bool compositionOpen(const QString& file);
    bool compositionSave(void);
    bool compositionSaveAs(void);
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert(void);
    bool compositionInsert(const QString& file);

private:
    class medComposerAreaPrivate *d;
};
