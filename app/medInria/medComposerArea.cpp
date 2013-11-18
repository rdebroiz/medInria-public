/* medComposerArea.cpp ---
 *
 * Author:
 * Created: Mon Nov 18 10:58:54 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 51
 */

/* Change Log:
 *
 */

#include "medComposerArea.h"

#include <dtkGui/dtkSplitter.h>

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerFactoryView.h>
#include <dtkComposer/dtkComposerSceneModel.h>
#include <dtkComposer/dtkComposerSceneNodeEditor.h>
#include <dtkComposer/dtkComposerSceneView.h>
#include <dtkComposer/dtkComposerStackView.h>
#include <dtkComposer/dtkComposerView.h>

#include <medComposer/medComposerFactory.h>

class medComposerAreaPrivate
{
public:
    dtkComposer *composer;
    dtkComposerFactoryView *nodes;
    dtkComposerSceneModel *model;
    dtkComposerSceneNodeEditor *editor;
    dtkComposerSceneView *scene;
    dtkComposerStackView *stack;
};

medComposerArea::medComposerArea(QWidget *parent) : QWidget(parent)
{
    d = new medComposerAreaPrivate;

    d->composer = new dtkComposer;
    d->composer->setFactory(new medComposerFactory);
    d->composer->view()->setCacheMode(QGraphicsView::CacheBackground);

    d->editor = new dtkComposerSceneNodeEditor(this);
    d->editor->setScene(d->composer->scene());
    d->editor->setStack(d->composer->stack());
    d->editor->setGraph(d->composer->graph());

    d->model = new dtkComposerSceneModel(this);
    d->model->setScene(d->composer->scene());

    d->scene = new dtkComposerSceneView(this);
    d->scene->setScene(d->composer->scene());
    d->scene->setModel(d->model);

    d->stack = new dtkComposerStackView(this);
    d->stack->setStack(d->composer->stack());

    d->nodes = new dtkComposerFactoryView(this);
    d->nodes->setFactory(d->composer->factory());
    d->nodes->setDark();

    // -- Layout

    dtkSplitter *left = new dtkSplitter(this);
    left->setOrientation(Qt::Vertical);
    left->addWidget(d->nodes);

    dtkSplitter *right = new dtkSplitter(this);
    right->setOrientation(Qt::Vertical);
    right->addWidget(d->scene);
    right->addWidget(d->editor);
    right->addWidget(d->stack);
    right->addWidget(d->composer->compass());
    right->setSizes(QList<int>()
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4
                    << this->size().height()/4);

    QWidget *middle = new QWidget(this);

    QVBoxLayout *m_layout = new QVBoxLayout(middle);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    m_layout->addWidget(d->composer);

    dtkSplitter *inner = new dtkSplitter(this);
    inner->setOrientation(Qt::Horizontal);
    inner->addWidget(left);
    inner->addWidget(middle);
    inner->addWidget(right);
    inner->setSizes(QList<int>()
                    << 200
                    << 1000
                    << 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(inner);
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
