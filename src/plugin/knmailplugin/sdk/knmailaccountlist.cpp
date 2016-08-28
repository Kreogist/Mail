/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QScrollBar>
#include <QTimeLine>
#include <QBoxLayout>

#include "knthememanager.h"

#include "knmailaccountmanager.h"
#include "knmailaccountwidget.h"

#include "knmailaccountlist.h"

#include <QDebug>

#define ScrollBarShow 150
#define ScrollBarHide 0

KNMailAccountList::KNMailAccountList(QWidget *parent) :
    KNSenseScrollArea(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_container(new QWidget(this)),
    m_containerLayout(new QBoxLayout(QBoxLayout::TopToBottom,
                                     m_container)),
    m_containerHeight(0),
    m_currentIndex(-1)
{
    setObjectName("MailAccountList");
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumWidth(256);
    knTheme->registerWidget(this);
    //Configure the scroll bar.
    customScrollBar()->setObjectName("MailAccountScrollBar");
    onActionMouseInOut(0);

    //Configure the account list gradient.
    QLinearGradient shadowGradient(QPoint(0, ItemHeight),
                                   QPoint(0, ItemHeight + ItemShadowHeight));
    shadowGradient.setColorAt(0, QColor(0,0,0,80));
    shadowGradient.setColorAt(1, QColor(0,0,0,0));
    KNMailAccountWidget::setShadowGradient(shadowGradient);

    //Configure the container.
    m_container->setContentsMargins(0, 0, 0, 0);
    setWidget(m_container);
    //Initial the layout for container.
    m_containerLayout->setContentsMargins(0,0,0,0);
    m_containerLayout->setSpacing(0);
    m_container->setLayout(m_containerLayout);
    //Add ended stretch.
    m_containerLayout->addStretch();
    //Configure the mouse time line.
    m_mouseAnime->setUpdateInterval(33);
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMailAccountList::onActionMouseInOut);
    //Get the latest account palette.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMailAccountList::onActionThemeChanged);
    onActionThemeChanged();
    //Link with the account manager.
    connect(knMailAccountManager, &KNMailAccountManager::accountAppended,
            this, &KNMailAccountList::onActionAccountAdded);
}

KNMailModel *KNMailAccountList::currentModel()
{
    //Check all the managed widget, there will be only one widget provides an
    //"un-null" pointer.
    for(auto i : m_accountList)
    {
        //Get the selected model.
        KNMailModel *selectedModel=i->currentModel();
        //Check the model.
        if(selectedModel)
        {
            //Give back the un-null model.
            return selectedModel;
        }
    }
    //If all of them provides null, then we will return a null pointer.
    return nullptr;
}

void KNMailAccountList::addAccountWidget(KNMailAccountWidget *accountWidget)
{
    //Check the container index.
    if(knMailAccountManager->accountCount()==1)
    {
        //Expand the first item.
        accountWidget->setExpand(true);
        //Set the current index to the first account.
        accountWidget->setCurrentIndex(0);
        m_currentIndex=0;
        //Load the inbox model.
        emit requireShowFolder(accountWidget->currentModel());
    }
    //Update the widget palette.
    accountWidget->setPalette(m_accountPalette);
    //Add account widget to the layout.
    m_containerLayout->insertWidget(m_containerLayout->count()-1,
                                    accountWidget);
    //Add the account widget to the list.
    m_accountList.append(accountWidget);
    //Calculate the container height.
    m_containerHeight+=accountWidget->height();
    //Resize the container.
    m_container->setFixedHeight(m_containerHeight);
    //Link the account widget expanded height.
    connect(accountWidget, &KNMailAccountWidget::panelExpanded,
            this, &KNMailAccountList::onActionPanelExpanded);
    connect(accountWidget, &KNMailAccountWidget::panelFolded,
            this, &KNMailAccountList::onActionPanelFold);
    //Link the show request to the manager's request.
    connect(accountWidget, &KNMailAccountWidget::requireShowFolder,
            this, &KNMailAccountList::onActionShowFolder);
    connect(accountWidget, &KNMailAccountWidget::panelSizeChange,
            this, &KNMailAccountList::onPanelSizeChange);
}

void KNMailAccountList::resizeEvent(QResizeEvent *event)
{
    //Do original event.
    KNSenseScrollArea::resizeEvent(event);
    //Update the container widget, sync with the account list.
    m_container->setFixedWidth(width());
}

void KNMailAccountList::enterEvent(QEvent *event)
{
    //Do original event.
    KNSenseScrollArea::enterEvent(event);
    //Start the scroll bar anime.
    startScrollAnime(ScrollBarShow);
}

void KNMailAccountList::leaveEvent(QEvent *event)
{
    //Do original event.
    KNSenseScrollArea::leaveEvent(event);
    //Start the scroll bar anime.
    startScrollAnime(ScrollBarHide);
}

void KNMailAccountList::onActionThemeChanged()
{
    //Update the account palette.
    m_accountPalette=knTheme->getPalette("MailAccountWidget");
    //Get all the widget in the list.
    for (auto i : m_accountList)
    {
        //Update the widget palette.
        i->setPalette(m_accountPalette);
    }
    //Update the scroll bar palette.
    customScrollBar()->setPalette(
                knTheme->getPalette(customScrollBar()->objectName()));
    onActionMouseInOut(0);
}

void KNMailAccountList::onActionAccountAdded(int accountIndex)
{
    //Add the account to the account list.
    addAccountWidget(new KNMailAccountWidget(
                         knMailAccountManager->account(accountIndex),
                         this));
}

void KNMailAccountList::onActionPanelExpanded(int panelHeight)
{
    //Update the container height counter.
    m_containerHeight+=panelHeight-ItemMargin-ItemHeight;
    //Start the animation.
    m_container->setFixedHeight(m_containerHeight);
}

void KNMailAccountList::onActionPanelFold()
{
    //Recast the sender.
    KNMailAccountWidget *senderWidget=
            static_cast<KNMailAccountWidget *>(sender());
    //Update the height indicator.
    m_containerHeight+=(ItemHeight+ItemMargin-senderWidget->expandedHeight());
    //Update the container height.
    m_container->setFixedHeight(m_containerHeight);
}

void KNMailAccountList::onActionMouseInOut(int opacity)
{
    //Get the scroll bar palette.
    QPalette pal=customScrollBar()->palette();
    //Change the button color.
    QColor color=pal.color(QPalette::Button);
    color.setAlpha(opacity);
    pal.setColor(QPalette::Button, color);
    //Change the base color.
    color=pal.color(QPalette::Base);
    color.setAlpha(opacity>>1);
    pal.setColor(QPalette::Base, color);
    //Update the palette of the scroll bar.
    customScrollBar()->setPalette(pal);
}

void KNMailAccountList::onActionShowFolder(KNMailModel *folder)
{
    //Check the new index folder.
    int senderIndex=
            m_accountList.indexOf(static_cast<KNMailAccountWidget *>(sender()));
    //Find the account item and save the current index.
    if(m_currentIndex!=senderIndex && m_currentIndex!=-1)
    {
        //Reset the previous index's current index.
        m_accountList.at(m_currentIndex)->setCurrentIndex(-1);
        //Update the current index.
        m_currentIndex=senderIndex;
    }
    //Recast the message.
    emit requireShowFolder(folder);
}

void KNMailAccountList::onPanelSizeChange(int previousSize, int panelSize)
{
    //Calculate the new size.
    m_containerHeight+=(panelSize-previousSize);
    //Update the fixed size.
    m_container->setFixedHeight(m_containerHeight);
}

inline void KNMailAccountList::startScrollAnime(int targetAlpha)
{
    //Start the time line.
    m_mouseAnime->stop();
    //Update the frame range.
    m_mouseAnime->setFrameRange(
                customScrollBar()->palette().color(QPalette::Button).alpha(),
                targetAlpha);
    //Start the time line.
    m_mouseAnime->start();
}
