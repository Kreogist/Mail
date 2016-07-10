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

KNMailAccountList::KNMailAccountList(QWidget *parent) :
    KNSenseScrollArea(parent),
    m_expandAnime(new QTimeLine(200, this)),
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
    knTheme->registerWidget(customScrollBar());
    //Configure the container.
    m_container->setContentsMargins(ItemSpacing, 0, ItemSpacing, 0);
    setWidget(m_container);
    //Initial the layout for container.
    m_containerLayout->setContentsMargins(0,0,0,0);
    m_containerLayout->setSpacing(0);
    m_container->setLayout(m_containerLayout);
    //Add ended stretch.
    m_containerLayout->addStretch();
    //Configure the time line.
    m_expandAnime->setUpdateInterval(33);
    m_expandAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_expandAnime, &QTimeLine::frameChanged,
            this, &KNMailAccountList::onActionChangeHeight);
    //Get the latest account palette.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMailAccountList::onActionThemeChanged);
    onActionThemeChanged();
    //Link with the account manager.
    connect(knMailAccountManager, &KNMailAccountManager::accountAppended,
            this, &KNMailAccountList::onActionAccountAdded);
}

void KNMailAccountList::addAccountWidget(KNMailAccountWidget *accountWidget)
{
    //Check the container index.
    if(knMailAccountManager->accountCount()==1)
    {
        //Expand the first item.
        accountWidget->setExpand(true);
        //Set the current index to the first account.
        m_currentIndex=0;
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
}

void KNMailAccountList::resizeEvent(QResizeEvent *event)
{
    //Do original event.
    KNSenseScrollArea::resizeEvent(event);
    //Update the container widget, sync with the account list.
    m_container->setFixedWidth(width());
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
    //Check the current index.
    if(m_currentIndex>-1)
    {
        //Fold the old item.
        m_accountList.at(m_currentIndex)->foldPanel();
    }
    //Find the account item and save the current index.
    m_currentIndex=
            m_accountList.indexOf(static_cast<KNMailAccountWidget *>(sender()));
    //Update the container height counter.
    m_containerHeight=panelHeight +
            (knMailAccountManager->accountCount() - 1) *
            (ItemHeight + ItemSpacing);
    //Start the animation.
    startAnime(m_containerHeight);
}

void KNMailAccountList::onActionChangeHeight(int containerHeight)
{
    //Update the container height.
    m_container->setFixedHeight(containerHeight);
}

inline void KNMailAccountList::startAnime(int targetHeight)
{
    //Start the time line.
    m_expandAnime->stop();
    //Update the frame range.
    m_expandAnime->setFrameRange(m_container->height(),
                                 targetHeight);
    //Start the anime.
    m_expandAnime->start();
}
