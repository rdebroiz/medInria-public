/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medHomepageArea.h>
#ifdef MEDINRIA_HAS_REVISIONS
#include <medRevisions.h>
#endif

#include <QToolButton>

#include <medSettingsManager.h>
#include <medPluginWidget.h>
#include <medSettingsEditor.h>


medHomePageCentralWidget::medHomePageCentralWidget(QWidget *parent): QWidget(parent)
{
    QSignalMapper *signalMapper = new QSignalMapper(this);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    this->setLayout(mainLayout);

    QWidget *leftPannelWidget = new QWidget;
    QVBoxLayout *leftPannelLayout = new QVBoxLayout;
    leftPannelWidget->setLayout(leftPannelLayout);

    QStackedWidget *stackedWidgets = new QStackedWidget;
    connect(signalMapper, SIGNAL(mapped(QWidget*)),
            stackedWidgets, SLOT(setCurrentWidget(QWidget*)));

    leftPannelLayout->addSpacing(20);
    QTextEdit * textEdit = new QTextEdit(this);
    textEdit->setHtml(tr("<p><b>medInria</b> is a cross-platform medical image "
                         "processing and visualisation software, "
                         "and it is <b>free</b>.</p><p>Through an intuitive user "
                         "interface, <b>medInria</b> offers from standard "
                         "to cutting-edge processing functionalities for "
                         "your medical images such as 2D/3D/4D image "
                         "visualisation, image registration, or diffusion "
                         "MR processing and tractography.</p>"));
    textEdit->setReadOnly (true);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    leftPannelLayout->addWidget(textEdit);
    QLabel * medInriaLabel = new QLabel;
    medInriaLabel->setPixmap(QPixmap(":pixmaps/medInria-logo-homepage.png"));
    textEdit->setMaximumWidth(medInriaLabel->width());
    leftPannelLayout->addWidget(medInriaLabel);
    leftPannelLayout->addSpacing(30);

    QToolButton * settingsButton = new QToolButton;
    settingsButton->setText(tr("Settings"));
    settingsButton->setMinimumHeight(50);
    settingsButton->setMinimumWidth(medInriaLabel->width());
    settingsButton->setMaximumWidth(medInriaLabel->width());
    settingsButton->setToolTip(tr("Configure medInria"));
    settingsButton->setFocusPolicy(Qt::NoFocus);
    settingsButton->setIcon(QIcon(":icons/settings.svg"));
    settingsButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    settingsButton->setObjectName("medHomePageButton");
    leftPannelLayout->addWidget(settingsButton);
    leftPannelLayout->addSpacing(30);

    medSettingsEditor* settingsEditor = new medSettingsEditor(NULL, true);
    settingsEditor->setTabPosition(QTabWidget::North);
    settingsEditor->initialize();
    settingsEditor->queryWidgets();
    stackedWidgets->addWidget(settingsEditor);
    signalMapper->setMapping(settingsButton, settingsEditor);
    connect(settingsButton, SIGNAL(clicked(bool)),
            signalMapper, SLOT(map()));

    QToolButton * pluginButton = new QToolButton;
    pluginButton->setText(tr("Plugins"));
    pluginButton->setMinimumHeight(50);
    pluginButton->setMinimumWidth(medInriaLabel->width());
    pluginButton->setMaximumWidth(medInriaLabel->width());
    pluginButton->setToolTip(tr("Information on loaded plugins"));
    pluginButton->setFocusPolicy(Qt::NoFocus);
    pluginButton->setIcon(QIcon(":icons/medInriaPlugin.png"));
    pluginButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    pluginButton->setObjectName("medHomePageButton");
    leftPannelLayout->addWidget(pluginButton);
    leftPannelLayout->addSpacing(30);

    medPluginWidget * pluginWidget = new medPluginWidget;
    stackedWidgets->addWidget(pluginWidget);
    signalMapper->setMapping(pluginButton, pluginWidget);
    connect(pluginButton, SIGNAL(clicked(bool)),
            signalMapper, SLOT(map()));

    QToolButton * aboutButton = new QToolButton;
    aboutButton->setText(tr("About"));
    aboutButton->setMinimumHeight(50);
    aboutButton->setMinimumWidth(medInriaLabel->width());
    aboutButton->setMaximumWidth(medInriaLabel->width());
    aboutButton->setToolTip(tr("About medInria"));
    aboutButton->setFocusPolicy(Qt::NoFocus);
    aboutButton->setIcon(QIcon(":icons/about.png"));
    aboutButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    aboutButton->setObjectName("medHomePageButton");
    leftPannelLayout->addWidget(aboutButton);
    leftPannelLayout->addSpacing(30);

    QTabWidget* aboutTabWidget = new QTabWidget;
    aboutTabWidget->setObjectName("aboutTabWidget");
    QTextEdit * aboutTextEdit = new QTextEdit;
    QString aboutText = QString(tr("<br/><br/>"
                                   "medInria %1 is a medical imaging platform developed at "
                                   "Inria<br/><br/>"
                                   "Inria, Copyright 2013"))
            .arg(qApp->applicationVersion());
#ifdef MEDINRIA_HAS_REVISIONS
    aboutText += QString::fromLocal8Bit(REVISIONS);
#endif


    aboutTextEdit->setHtml(aboutText);
    aboutTextEdit->setFocusPolicy ( Qt::NoFocus );
    QTextBrowser * aboutAuthorTextBrowser = new QTextBrowser;
    aboutAuthorTextBrowser->setSource(QUrl("qrc:authors.html" ));
    aboutAuthorTextBrowser->setFocusPolicy ( Qt::NoFocus );

    QTextEdit * aboutLicenseTextEdit = new QTextEdit;
    QFile license(":LICENSE.txt");
    license.open(QIODevice::ReadOnly | QIODevice::Text);
    QString licenseContent = license.readAll();
    license.close();
    aboutLicenseTextEdit->setText(licenseContent);
    aboutLicenseTextEdit->setFocusPolicy(Qt::NoFocus);

    QTextEdit * releaseNotesTextEdit = new QTextEdit;
    QFile releaseNotes(":RELEASE_NOTES.txt");
    releaseNotes.open(QIODevice::ReadOnly | QIODevice::Text);
    QString releaseNotesContent = releaseNotes.readAll();
    releaseNotes.close();
    releaseNotesTextEdit->setText(releaseNotesContent);
    releaseNotesTextEdit->setFocusPolicy(Qt::NoFocus);

    aboutTabWidget->addTab(aboutTextEdit, tr("About"));
    aboutTabWidget->addTab(aboutAuthorTextBrowser, tr("Authors"));
    aboutTabWidget->addTab(releaseNotesTextEdit, tr("Release Notes"));
    aboutTabWidget->addTab(aboutLicenseTextEdit, tr("License"));

    stackedWidgets->addWidget(aboutTabWidget);
    signalMapper->setMapping(aboutButton, aboutTabWidget);
    connect(aboutButton, SIGNAL(clicked(bool)),
            signalMapper, SLOT(map()));

    //User widget content with settings, about and help buttons
    QToolButton * helpButton = new QToolButton;
    helpButton->setText ( "Help" );
    helpButton->setToolTip(tr("Open Online Documentation"));
    helpButton->setMinimumHeight(50);
    helpButton->setMinimumWidth(medInriaLabel->width());
    helpButton->setMaximumWidth(medInriaLabel->width());
    helpButton->setFocusPolicy(Qt::NoFocus);
    helpButton->setIcon(QIcon(":icons/help.svg"));
    helpButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    helpButton->setObjectName("medHomePageButton");
    connect(helpButton, &QToolButton::clicked,
            this, &medHomePageCentralWidget::showHelpRequest);
    leftPannelLayout->addWidget(helpButton);

    leftPannelLayout->addStretch();

    mainLayout->addWidget(leftPannelWidget);
    mainLayout->addWidget(stackedWidgets);
}

medHomePageCentralWidget::~medHomePageCentralWidget()
{
}


medHomepageArea::medHomepageArea(QObject * parent): medAbstractArea(parent)
{

}

medHomepageArea::~medHomepageArea()
{
}

QWidget * medHomepageArea::centralWidget() const
{
    medHomePageCentralWidget* centralWidget = new medHomePageCentralWidget;
    connect(centralWidget, &medHomePageCentralWidget::showHelpRequest,
            this, &medHomepageArea::showHelp);
    return centralWidget;
}


QWidget * medHomepageArea::statusBarWidget() const
{
    return new QWidget;
}

void medHomepageArea::showHelp() const
{
    QDesktopServices::openUrl(QUrl("http://med.inria.fr/help/documentation"));
}
