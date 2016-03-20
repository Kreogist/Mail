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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "kmmainwindowleftcontainer.h"

#include "kmmainwindowcontainer.h"

#include <QDebug>

KMMainWindowContainer::KMMainWindowContainer(QWidget *parent) :
    QSplitter(parent),
    m_leftContainer(new KMMainWindowLeftContainer(this)),
    m_mailComponent(nullptr)
{
    //Add left container.
    addWidget(m_leftContainer);
}

void KMMainWindowContainer::setMailComponent(QWidget *mailComponent)
{
    //Save the pointer.
    m_mailComponent = mailComponent;
    //Check the widget pointer.
    if(!m_mailComponent)
    {
        //Pointer is nullptr.
        return;
    }
    //Add widget to splitter.
    addWidget(m_mailComponent);
}

void KMMainWindowContainer::setUniBar(QWidget *uniBar)
{
    //Give the pointer to left container.
    m_leftContainer->setUniBar(uniBar);
}

void KMMainWindowContainer::setMailList(QWidget *mailList)
{
    //Give the pointer to left container.
    m_leftContainer->setMailList(mailList);
}

void KMMainWindowContainer::setTitleBar(QWidget *titleBar)
{
    //Give the pointer to left container.
    m_leftContainer->setTitleBar(titleBar);
}
