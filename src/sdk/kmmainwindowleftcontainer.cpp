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
#include <QBoxLayout>

#include "kmmainwindowleftcontainer.h"

KMMainWindowLeftContainer::KMMainWindowLeftContainer(QWidget *parent) :
    QWidget(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_contentLayout(new QBoxLayout(QBoxLayout::LeftToRight,
                                   m_mainLayout->widget()))
{
    //Configure the main layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    //Configure the content layout.
    m_contentLayout->setContentsMargins(0,0,0,0);
    m_contentLayout->setSpacing(0);
    //Set the layout.
    setLayout(m_mainLayout);
    //Add content layout to main layout.
    m_mainLayout->addLayout(m_contentLayout, 1);
}

void KMMainWindowLeftContainer::setTitleBar(QWidget *titleBar)
{
    //Check pointer has been saved before.
    if(m_titleBar)
    {
        //Ignore the twice settting.
        return;
    }
    //Save the title bar widget.
    m_titleBar = titleBar;
    //Check pointer.
    if(!m_titleBar)
    {
        //Ignore the nullptr widget.
        return;
    }
    //Add widget to layout.
    m_mainLayout->insertWidget(0, m_titleBar);
}

void KMMainWindowLeftContainer::setMailList(QWidget *mailList)
{
    //Check pointer has been saved before.
    if(m_mailList)
    {
        //Ignore the twice settting.
        return;
    }
    //Save the widget pointer.
    m_mailList = mailList;
    //Check pointer first.
    if(!m_mailList)
    {
        //Ignore the mail list widget.
        return;
    }
    //Add mail list to content layout.
    m_contentLayout->addWidget(m_mailList);
}

void KMMainWindowLeftContainer::setUniBar(QWidget *uniBar)
{
    //Check pointer has been saved before.
    if(m_uniBar)
    {
        //Ignore the twice settting.
        return;
    }
    //Save the unibar.
    m_uniBar = uniBar;
    //Check the unibar pointer.
    if(!m_uniBar)
    {
        //Ignore the nullptr widget.
        return;
    }
    //Hide the unibar.
    m_uniBar->hide();
}
