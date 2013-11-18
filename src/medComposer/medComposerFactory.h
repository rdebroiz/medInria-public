/* medComposerFactory.h ---
 *
 * Author:
 * Created: Mon Nov 18 10:48:55 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 10
 */

/* Change Log:
 *
 */

#pragma once

#include <dtkComposer/dtkComposerFactory.h>

class medComposerFactory : public dtkComposerFactory
{
    Q_OBJECT

public:
     medComposerFactory(void);
    ~medComposerFactory(void);

public slots:
    dtkComposerNode *create(const QString& type);

public:
    QList<QString> nodes(void);
    QHash<QString, QString> descriptions(void);
    QHash<QString, QStringList> tags(void);
    QHash<QString, QString> types(void);

private:
    class medComposerFactoryPrivate *d;
};
