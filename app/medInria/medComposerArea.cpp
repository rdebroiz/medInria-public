/* medComposerArea.cpp ---
 *
 * Author:
 * Created: Mon Nov 18 10:58:54 2013 (+0100)
 * Version:
 * Last-Updated:
 *           By:
 *     Update #: 168
 */

/* Change Log:
 *
 */

#include "medComposerArea.h"

#include <dtkGui/dtkSplitter.h>
#include <dtkGui/dtkToolBox.h>

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerFactoryView.h>
#include <dtkComposer/dtkComposerSceneModel.h>
#include <dtkComposer/dtkComposerSceneNodeEditor.h>
#include <dtkComposer/dtkComposerSceneView.h>
#include <dtkComposer/dtkComposerStackView.h>
#include <dtkComposer/dtkComposerView.h>

#include <dtkDistributed/dtkDistributor.h>

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

public:
    QString current_composition;

public:
    QPushButton *button_open;
    QPushButton *button_save;
    QPushButton *button_save_as;
    QPushButton *button_insert;

public:
    QPushButton *button_start;
    QPushButton *button_stop;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

medComposerArea::medComposerArea(QWidget *parent) : QWidget(parent)
{
    d = new medComposerAreaPrivate;

    d->button_open = new QPushButton("Open");
    d->button_open->setObjectName("left");
    d->button_save = new QPushButton("Save");
    d->button_save->setObjectName("center");
    d->button_save_as = new QPushButton("Save As");
    d->button_save_as->setObjectName("center");
    d->button_insert = new QPushButton("Insert");
    d->button_insert->setObjectName("right");

    QHBoxLayout *f_layout = new QHBoxLayout;
    f_layout->setSpacing(0);
    f_layout->addWidget(d->button_open);
    f_layout->addWidget(d->button_save);
    f_layout->addWidget(d->button_save_as);
    f_layout->addWidget(d->button_insert);

    QFrame *f_menu = new QFrame(this);
    f_menu->setLayout(f_layout);

    connect(d->button_open,    SIGNAL(clicked()), this, SLOT(compositionOpen()));
    connect(d->button_save,    SIGNAL(clicked()), this, SLOT(compositionSave()));
    connect(d->button_save_as, SIGNAL(clicked()), this, SLOT(compositionSaveAs()));
    connect(d->button_insert,  SIGNAL(clicked()), this, SLOT(compositionInsert()));

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

// ///////////////////////////////////////////////////////////////////

    d->button_start = new QPushButton("Start");
    d->button_start->setObjectName("left");
    d->button_stop = new QPushButton("Stop");
    d->button_stop->setObjectName("right");

    QHBoxLayout *c_layout = new QHBoxLayout;
    c_layout->setSpacing(0);
    c_layout->addWidget(d->button_start);
    c_layout->addWidget(d->button_stop);

    QFrame *c_menu = new QFrame(this);
    c_menu->setLayout(c_layout);

    connect(d->button_start, SIGNAL(clicked()), d->composer, SLOT(run()));
    connect(d->button_stop,  SIGNAL(clicked()), d->composer, SLOT(stop()));

    dtkDistributor *distributor = new dtkDistributor(this);
    distributor->setApplication("medSlave");

    // -- Layout

    dtkSplitter *left = new dtkSplitter(this);
    left->setOrientation(Qt::Vertical);

    left->addWidget(f_menu);
    left->addWidget(c_menu);
    left->addWidget(d->nodes);
    left->addWidget(distributor);

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
                    << 300
                    << 1000
                    << 300);

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

bool medComposerArea::compositionOpen(void)
{
    QFileDialog *dialog = new QFileDialog(this, tr("Open composition"), QDir::homePath(), QString("medinria composition (*.med)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionOpen(const QString&)));

    return true;
}

bool medComposerArea::compositionOpen(const QString& file)
{
    bool status = d->composer->open(file);

    return status;
}

bool medComposerArea::compositionSave(void)
{
    bool status;

    if(d->current_composition.isEmpty() || d->current_composition == "untitled.med")
        status = this->compositionSaveAs();
    else
        status = d->composer->save();

    if(status)
        this->setWindowModified(false);

    return status;
}

bool medComposerArea::compositionSaveAs(void)
{
    bool status = false;

    QStringList nameFilters;
    nameFilters <<  "Ascii composition (*.med)";
    nameFilters << "Binary composition (*.med)";

    QFileDialog dialog(this, "Save composition as ...", QDir::homePath(), QString("medinria composition (*.med)"));
    dialog.setStyleSheet("background-color: none ; color: none;");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setConfirmOverwrite(true);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilters(nameFilters);
    dialog.setDefaultSuffix("med");

    if(dialog.exec()) {

        if(dialog.selectedNameFilter() == nameFilters.at(0))
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Ascii);
        else
            status = this->compositionSaveAs(dialog.selectedFiles().first(), dtkComposerWriter::Binary);
    }

    return status;
}

bool medComposerArea::compositionSaveAs(const QString& file, dtkComposerWriter::Type type)
{
    bool status = false;

    if(file.isEmpty())
        return status;

    status = d->composer->save(file, type);

    if(status) {
        d->current_composition = file;
        this->setWindowModified(false);
    }

    return status;
}

bool medComposerArea::compositionInsert(void)
{
    QFileDialog *dialog = new QFileDialog(this, tr("Insert composition"), QDir::homePath(), QString("medinria composition (*.med)"));
    dialog->setStyleSheet("background-color: none ; color: none;");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->open(this, SLOT(compositionInsert(const QString&)));

    return true;
}

bool medComposerArea::compositionInsert(const QString& file)
{
    bool status = d->composer->insert(file);

    if(status)
        this->setWindowModified(true);

    return status;
}
