/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medQuickAccessMenu.h>
#include <medWorkspaceFactory.h>
#include <medSettingsManager.h>

#include <medGuiLayer.h>

#include <medAbstractArea.h>

#ifdef Q_OS_MAC
    Qt::Key OSIndependentControlKey = Qt::Key_Meta;
#else
    Qt::Key OSIndependentControlKey = Qt::Key_Control;
#endif

class medQuickAccessMenuPrivate
{
public:
    unsigned int currentSelected;

    QList<medHomepagePushButton *> buttonsList;

    //! Frame for alt-tab like shortcut access menu
    QFrame * backgroundFrame;
    QVBoxLayout* verticalButtonsLayout;
    QHBoxLayout* horizontalButtonsLayout;

    bool vertical;

    QSignalMapper *signalmap;
};


/**
 * Constructor, parameter vertical chooses if the layout will be vertical (bottom left menu) or horizontal (alt-tab like menu)
 */
medQuickAccessMenu::medQuickAccessMenu (bool vertical, QWidget* parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    d(new medQuickAccessMenuPrivate)
{
    d->currentSelected = 0;
    d->backgroundFrame = NULL;

//    QVariant startupWorkspace = medSettingsManager::instance()->value("startup","default_starting_area");
//    d->currentSelected = startupWorkspace.toInt();

    d->signalmap = new QSignalMapper(this);
//    connect(d->signalmap, &QSignalMapper::mapped,
//            this, &medQuickAccessMenu::areaSelected);

    connect(d->signalmap, SIGNAL(mapped(QString)),
            this, SIGNAL(areaSelected(QString)));

    d->vertical = vertical;

    if (d->vertical)
    {
        d->verticalButtonsLayout = new QVBoxLayout;
        d->verticalButtonsLayout->setMargin(0);
        d->verticalButtonsLayout->setSpacing(0);

        //Setup quick access menu title
        QLabel * headerLabel = new QLabel(tr("<b>Switch to areas</b>"));
        headerLabel->setMaximumWidth(300);
        headerLabel->setFixedHeight(25);
        headerLabel->setAlignment(Qt::AlignCenter);
        headerLabel->setTextFormat(Qt::RichText);
        headerLabel->setObjectName("quickAccessMenuHeader");
        d->verticalButtonsLayout->addWidget(headerLabel);
    }

    else
    {
        unsigned int nbAreas = medGuiLayer::area::pluginFactory().keys().size();

        d->backgroundFrame = new QFrame(this);
        d->backgroundFrame->setStyleSheet("border-radius: 10px;background-color: rgba(200,200,200,150);");
        QHBoxLayout *mainWidgetLayout = new QHBoxLayout;
        mainWidgetLayout->setMargin(0);
        mainWidgetLayout->setSpacing(0);

        d->horizontalButtonsLayout = new QHBoxLayout;
        d->backgroundFrame->setLayout(d->horizontalButtonsLayout);
        d->backgroundFrame->setFixedWidth(40 + 180 * ( 2 + nbAreas));
        d->backgroundFrame->setFixedHeight(240);
        d->backgroundFrame->setMouseTracking(true);
        mainWidgetLayout->addWidget(d->backgroundFrame);
        this->setFixedWidth(40 + 180 * (2 + nbAreas));
        this->setFixedHeight(240);
        this->setLayout(mainWidgetLayout);

        this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->setAttribute(Qt::WA_TranslucentBackground, true);
    }
}

/**
 *  emit the menuHidden() signal when the widget lost the focus
 */
void medQuickAccessMenu::focusOutEvent ( QFocusEvent* event )
{
    QWidget::focusOutEvent ( event );
    emit menuHidden();
}

/**
 * Handles key press events to control selected widget in menu
 */
void medQuickAccessMenu::keyPressEvent ( QKeyEvent * event )
{
    if ((event->key() == Qt::Key_Down)||(event->key() == Qt::Key_Right))
    {
        this->updateCurrentlySelectedRight();
        return;
    }

    if ((event->key() == Qt::Key_Up)||(event->key() == Qt::Key_Left))
    {
        this->updateCurrentlySelectedLeft();
        return;
    }

    if (event->key() == Qt::Key_Return)
    {
        this->switchToCurrentlySelected();
        return;
    }

    if (event->key() == Qt::Key_Escape)
    {
        emit menuHidden();
        return;
    }

    QApplication::sendEvent(this->parentWidget(),event);
}

/**
 * Send back key release event to parent widget that handles key releases
 */
void medQuickAccessMenu::keyReleaseEvent ( QKeyEvent * event )
{
    if (event->key() == OSIndependentControlKey && this->isVisible())
    {
        emit menuHidden();
        switchToCurrentlySelected();
        return;
    }

    if (event->key() == Qt::Key_Space && event->modifiers().testFlag(Qt::ShiftModifier))
    {
        updateCurrentlySelectedLeft();
        return;
    }

    QApplication::sendEvent(this->parentWidget(),event);
}

/**
 * Mouse over implementation to select widget in menu
 */
void medQuickAccessMenu::mouseMoveEvent (QMouseEvent *event)
{
    for (int i = 0;i < d->buttonsList.size();++i)
    {
        QRect widgetRect = d->buttonsList[i]->geometry();
        if (widgetRect.contains(event->pos()))
        {
            this->mouseSelectWidget(i);
            break;
        }
    }

    QWidget::mouseMoveEvent(event);
}

/**
 * Updates the currently selected item when current workspace was changed from somewhere else
 */
void medQuickAccessMenu::updateSelected(QString area)
{
    d->currentSelected = 0;

    for (int i = 0;i < d->buttonsList.size();++i)
    {
        if (d->buttonsList[i]->identifier() == area)
        {
            d->currentSelected = i;
            break;
        }
    }
}

/**
 * Actually emits signal to switch to selected workspace
 */
void medQuickAccessMenu::switchToCurrentlySelected()
{
    if(!d->buttonsList.isEmpty())
        emit areaSelected(d->buttonsList[d->currentSelected]->identifier());
}

/**
 * Move selected widget one position to the left
 */
void medQuickAccessMenu::updateCurrentlySelectedLeft()
{
    if (d->currentSelected <= 0)
    {
        if (d->currentSelected == 0)
            d->buttonsList[d->currentSelected]->setSelected(false);

        d->currentSelected = d->buttonsList.size() - 1;
        d->buttonsList[d->currentSelected]->setSelected(true);

        return;
    }

    d->buttonsList[d->currentSelected]->setSelected(false);
    d->currentSelected--;
    d->buttonsList[d->currentSelected]->setSelected(true);
}

/**
 * Move selected widget one position to the right
 */
void medQuickAccessMenu::updateCurrentlySelectedRight()
{
    if (d->currentSelected < 0)
    {
        d->currentSelected = 0;
        d->buttonsList[d->currentSelected]->setSelected(true);
        return;
    }

    d->buttonsList[d->currentSelected]->setSelected(false);

    d->currentSelected++;
    if (d->currentSelected >= d->buttonsList.size())
        d->currentSelected = 0;

    d->buttonsList[d->currentSelected]->setSelected(true);
}

/**
 * Resets the menu when shown, optionally updates the layout to window size
 */
void medQuickAccessMenu::reset(bool optimizeLayout)
{
    for (int i = 0;i < d->buttonsList.size();++i)
    {
        if (d->currentSelected == i)
            d->buttonsList[i]->setSelected(true);
        else
            d->buttonsList[i]->setSelected(false);
    }

    if ((optimizeLayout)&&(d->backgroundFrame))
    {
        unsigned int width = ((QWidget *)this->parent())->size().width();
        unsigned int numberOfWidgetsPerLine = floor((width - 40.0) / 180.0);
        unsigned int optimalSizeLayout = ceil((float)d->buttonsList.size() / numberOfWidgetsPerLine);

        if (d->backgroundFrame->layout())
            delete d->backgroundFrame->layout();

        if (optimalSizeLayout > 1)
        {
            QGridLayout *layout = new QGridLayout;
            unsigned int totalAdded = 0;
            for (unsigned int i = 0;i < optimalSizeLayout;++i)
            {
                for (unsigned int j = 0;j < numberOfWidgetsPerLine;++j)
                {
                    layout->addWidget(d->buttonsList[totalAdded],i,j);
                    ++totalAdded;

                    if (totalAdded == (unsigned int)d->buttonsList.size())
                        break;
                }

                if (totalAdded == (unsigned int)d->buttonsList.size())
                    break;
            }

            d->backgroundFrame->setLayout(layout);
            d->backgroundFrame->setFixedWidth(40 + 180 * numberOfWidgetsPerLine );
            d->backgroundFrame->setFixedHeight(130 * optimalSizeLayout );
            this->setFixedWidth(40 + 180 * numberOfWidgetsPerLine );
            this->setFixedHeight(130 * optimalSizeLayout );
        }
        else
        {
            QHBoxLayout *layout = new QHBoxLayout;
            for (int i = 0;i < d->buttonsList.size();++i)
                layout->addWidget(d->buttonsList[i]);

            d->backgroundFrame->setLayout(layout);
            d->backgroundFrame->setFixedWidth(40 + 180 * d->buttonsList.size() );
            d->backgroundFrame->setFixedHeight(130 );

            this->setFixedWidth (40 + 180 * d->buttonsList.size() );
            this->setFixedHeight (130);
        }
    }
}

/**
 * Mouse over slot to select widget in menu
 */
void medQuickAccessMenu::mouseSelectWidget(unsigned int identifier)
{
    unsigned int newSelection = identifier;

    if (newSelection == (unsigned int)d->currentSelected)
        return;

    if (d->currentSelected >= 0)
        d->buttonsList[d->currentSelected]->setSelected(false);

    d->buttonsList[newSelection]->setSelected(true);
    d->currentSelected = newSelection;
}


void medQuickAccessMenu::addArea(medAbstractArea *area)
{
    medHomepagePushButton * button = new medHomepagePushButton(this);
    QString areaId = area->metaObject()->className();
    button->setText(area->title());
    button->setIdentifier(areaId);
    button->setIcon(area->icon());

    if(d->vertical)
    {
        button->setFixedHeight(40);
        button->setMaximumWidth(250);
        button->setMinimumWidth(250);
        button->setStyleSheet("border: 0px;");
        button->setFocusPolicy (Qt::NoFocus);
        button->setCursor(Qt::PointingHandCursor);
        d->verticalButtonsLayout->addWidget(button);

    }
    else
    {
        button->setFixedHeight ( 100 );
        button->setFixedWidth ( 160 );
        button->setStyleSheet("border-radius: 5px;font-size:12px;color: #ffffff;");
        /*background-image: url(:icons/home_sc.png) no-repeat;*/
        button->setFocusPolicy ( Qt::NoFocus );
        button->setCursor(Qt::PointingHandCursor);
        button->setSelected(true);
        d->horizontalButtonsLayout->addWidget(button);
    }

//    connect(button, &medHomepageButton::clicked,
//            d->signalmap, &QSignalMapper::map);
    connect(button, SIGNAL(clicked(bool)),
            d->signalmap, SLOT(map()));
    d->signalmap->setMapping(button, areaId);
    d->buttonsList << button;
}
