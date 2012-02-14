#include "medFileSystemDataSource.h"

#include <dtkCore/dtkGlobal.h>
#include <dtkGui/dtkFinder.h>

#include <medToolBoxActions.h>

class medFileSystemDataSourcePrivate
{
public:
    QWidget *filesystem_widget;
    dtkFinder *finder;
    dtkFinderPathBar *path;
    dtkFinderSideView *side;
    dtkFinderToolBar *toolbar;

    QList<medToolBox*> toolboxes;
    medToolBoxActions* actionsTb;
};

medFileSystemDataSource::medFileSystemDataSource( QWidget* parent /*= 0*/ ): medAbstractDataSource(parent), d(new medFileSystemDataSourcePrivate)
{
    d->filesystem_widget = new QWidget(parent);

    d->finder = new dtkFinder (d->filesystem_widget);
    d->finder->setPath(QDir::homePath());
    d->finder->allowFileBookmarking(false);
    d->finder->allowMultipleSelection(true);

    d->path = new dtkFinderPathBar (d->filesystem_widget);
    d->path->setPath(QDir::homePath());

    d->toolbar = new dtkFinderToolBar (d->filesystem_widget);
    d->toolbar->setPath(QDir::currentPath());

    d->actionsTb = new medToolBoxActions(parent);
    d->toolboxes.push_back(d->actionsTb);

    d->side = new dtkFinderSideView;
    d->side->setStyleSheet(
        "dtkFinderSideView {"
        "    color: #b2b2b2;"
        "    padding: 5px;"
        "    background: #494949;"
        "    show-decoration-selected: 1;"
        "}"
        ""
        "dtkFinderSideView::item {"
        "    margin-left: 0px;"
        "    border-top-color: transparent;"
        "    border-bottom-color: transparent;"
        "}"
        ""
        "dtkFinderSideView::item:selected {"
        "    border-top: 1px solid #567dbc;"
        "    border-bottom: 1px solid #567dbc;"
        "}"
        ""
        "dtkFinderSideView::item:selected:active{"
        "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc);"
        "}"
        ""
        "dtkFinderSideView::item:selected:!active {"
        "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf);"
        "}"
        ""
        "dtkFinderSideView::branch:selected {"
        "    border-top: 1px solid #567dbc;"
        "    border-bottom: 1px solid #567dbc;"
        "}"
        ""
        "dtkFinderSideView::branch:selected:active{"
        "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc);"
        "}"
        ""
        "dtkFinderSideView::branch:selected:!active {"
        "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6b9be8, stop: 1 #577fbf);"
        "}");

    QAction *importAction = new QAction(tr("Import"), this);
    QAction *indexAction = new QAction(tr("Index"), this);
    QAction *loadAction = new QAction(tr("Load"), this);
    QAction *viewAction = new QAction(tr("View"), this);

    d->finder->addContextMenuAction(importAction);
    d->finder->addContextMenuAction(indexAction);
    d->finder->addContextMenuAction(loadAction);
    d->finder->addContextMenuAction(viewAction);

    connect(importAction, SIGNAL(triggered()), this, SLOT(onFileSystemImportRequested()));
    connect(indexAction, SIGNAL(triggered()),  this, SLOT(onFileSystemIndexRequested()));
    connect(  loadAction, SIGNAL(triggered()), this, SLOT(onFileSystemLoadRequested()));
    connect(  viewAction, SIGNAL(triggered()), this, SLOT(onFileSystemViewRequested()));

    QHBoxLayout *toolbar_layout = new QHBoxLayout;
    toolbar_layout->setContentsMargins(0, 0, 0, 0);
    toolbar_layout->setSpacing(0);
    toolbar_layout->addWidget  (d->toolbar);
    toolbar_layout->addWidget  (d->path);

    QVBoxLayout *filesystem_layout = new QVBoxLayout(d->filesystem_widget);
    filesystem_layout->setContentsMargins(10, 10, 10, 10);
    filesystem_layout->setSpacing(0);
    filesystem_layout->addLayout (toolbar_layout);
    filesystem_layout->addWidget(d->finder);

    connect(d->finder, SIGNAL(changed(QString)), d->path, SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->side, SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(fileDoubleClicked(const QString&)), this, SLOT(onFileDoubleClicked(const QString&)));

    connect(d->path, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->side, SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->side, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->side, SIGNAL(changed(QString)), d->path, SLOT(setPath(QString)));
    connect(d->side, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->finder, SIGNAL(bookmarked(QString)), d->side, SLOT(addBookmark(QString)));

    connect (d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect (d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));

    connect (d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect (d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));

    connect(d->finder, SIGNAL(selectionChanged(const QStringList&)), d->actionsTb, SLOT(selectedPathsChanged(const QStringList&)));

    connect(d->actionsTb, SIGNAL(bookmarkClicked()), d->finder, SLOT(onBookmarkSelectedItemsRequested()));
    connect(d->actionsTb, SIGNAL(viewClicked()), this, SLOT(onFileSystemViewRequested()));
    connect(d->actionsTb, SIGNAL(importClicked()), this, SLOT(onFileSystemImportRequested()));
    connect(d->actionsTb, SIGNAL(indexClicked()), this, SLOT(onFileSystemIndexRequested()));
    connect(d->actionsTb, SIGNAL(loadClicked()), this, SLOT(onFileSystemLoadRequested()));
}

medFileSystemDataSource::~medFileSystemDataSource()
{
    delete d;
    d = NULL;
}

QWidget* medFileSystemDataSource::mainViewWidget()
{
    return d->filesystem_widget;
}

QWidget* medFileSystemDataSource::sourceSelectorWidget()
{
    return d->side;
}

QString medFileSystemDataSource::tabName()
{
    return tr("File");
}

QList<medToolBox*> medFileSystemDataSource::getToolboxes()
{
    return d->toolboxes;
}

QString medFileSystemDataSource::description(void) const
{
	return tr("Browse the file system");
}

void medFileSystemDataSource::onFileSystemImportRequested(void)
{
    foreach(QString path, d->finder->selectedPaths())
    {
        QFileInfo info(path);
        emit dataToImportReceived(info.absoluteFilePath());
    }
}

void medFileSystemDataSource::onFileSystemIndexRequested(void)
{
    foreach(QString path, d->finder->selectedPaths())
    {
        QFileInfo info(path);
        emit dataToIndexReceived(info.absoluteFilePath());
    }
}

void medFileSystemDataSource::onFileSystemLoadRequested()
{
    foreach(QString path, d->finder->selectedPaths())
    {
        QFileInfo info(path);
        emit load(info.absoluteFilePath());
    }
}

void medFileSystemDataSource::onFileSystemViewRequested()
{
    foreach(QString path, d->finder->selectedPaths())
    {
        QFileInfo info(path);
        emit open(info.absoluteFilePath());
    }
}

void medFileSystemDataSource::onFileDoubleClicked(const QString& filename)
{
    QFileInfo info(filename);
    if (info.isFile())
        emit open(info.absoluteFilePath());
}
