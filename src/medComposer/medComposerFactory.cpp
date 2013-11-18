/* medComposerFactory.cpp ---
 *
 * Author:
 * Created: Mon Nov 18 10:49:57 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 26
 */

/* Change Log:
 *
 */

#include "medComposerFactory.h"
#include "medComposerNodeImage.h"
#include "medComposerNodeView.h"

#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewFactory.h>

#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class medComposerFactoryPrivate
{
public:
    void initialize(void);

public:
    QList<QString> nodes;
    QHash<QString, QString> descriptions;
    QHash<QString, QStringList> tags;
    QHash<QString, QString> types;
};

void medComposerFactoryPrivate::initialize(void)
{

}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

medComposerFactory::medComposerFactory(void) : dtkComposerFactory(), d(new medComposerFactoryPrivate)
{
    d->initialize();

    // Initialize attributes on super class

    d->nodes = QList<QString>(dtkComposerFactory::nodes());
    d->descriptions = QHash<QString, QString>(dtkComposerFactory::descriptions());
    d->tags = QHash<QString, QStringList>(dtkComposerFactory::tags());
    d->types = QHash<QString, QString>(dtkComposerFactory::types());

    // Extend attributes

    d->nodes << "View";
    d->descriptions["View"] = "<p>medAbstractView.</p>";
    d->tags["View"] = QStringList() << "view";
    d->types["View"] = "medAbstractView";

    d->nodes << "Image";
    d->descriptions["Image"] = "<p>medAbstractImage.</p>";
    d->tags["Image"] = QStringList() << "image" << "reader";
    d->types["Image"] = "medAbstractDataImage";
}

medComposerFactory::~medComposerFactory(void)
{
    delete d;

    d = NULL;
}

dtkComposerNode *medComposerFactory::create(const QString& type)
{
    if(type == "medAbstractView")
        return new medComposerNodeView;

    if(type == "medAbstractDataImage")
        return new medComposerNodeImage;

    return dtkComposerFactory::create(type);
}

QList<QString> medComposerFactory::nodes(void)
{
    return d->nodes;
}

QHash<QString, QString> medComposerFactory::descriptions(void)
{
    return d->descriptions;
}

QHash<QString, QStringList> medComposerFactory::tags(void)
{
    return d->tags;
}

QHash<QString, QString> medComposerFactory::types(void)
{
    return d->types;
}
