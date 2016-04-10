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
#include <QSizePolicy>

#include "knsideshadowwidget.h"

#include "kmmainwindowleftcontainer.h"

#define ShadowWidth 10

KMMainWindowLeftContainer::KMMainWindowLeftContainer(QWidget *parent) :
    QWidget(parent),
    m_rightShadow(new KNSideShadowWidget(KNSideShadowWidget::RightShadow,
                                         this)),
    m_titleBar(nullptr),
    m_mailList(nullptr),
    m_uniBar(nullptr)
{
    //Change the size policy.
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //Configure the left shadow.
    m_rightShadow->setFixedWidth(ShadowWidth);
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
    //Change the relationship.
    m_titleBar->setParent(this);
    //Raise the shadow.
    m_rightShadow->raise();
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
    //Change the relationship.
    m_mailList->setParent(this);
    //Raise the shadow.
    m_rightShadow->raise();
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
    //Change the parent relationship.
    m_uniBar->setParent(this);
    //Hide the unibar.
    m_uniBar->hide();
    //Raise the shadow.
    m_rightShadow->raise();
}

void KMMainWindowLeftContainer::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the shadow.
    m_rightShadow->setGeometry(width()-ShadowWidth,
                               0,
                               ShadowWidth,
                               height());
    //Resize the title bar is it's not null.
    if(m_titleBar)
    {
        //Resize title bar.
        m_titleBar->setGeometry(0,0,width(), m_titleBar->height());
    }
    //Resize the mail list.
    if(m_mailList)
    {
        //Resize the mail list.
        m_mailList->setGeometry(0, m_titleBar->height(),
                                width(),
                                height() - m_titleBar->height());
    }
}
