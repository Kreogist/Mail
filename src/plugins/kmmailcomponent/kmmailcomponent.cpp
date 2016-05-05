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
#include <QTimeLine>
#include <QScrollArea>
#include <QScrollBar>

#include "knthememanager.h"
#include "sao/knsaostyle.h"

#include "kmmailcomponenttitlebar.h"
#include "kmmailcomponentcontent.h"

#include "kmmailcomponent.h"

#include <QDebug>

#define ScrollBarWidth 10
#define MaxOpacity 0x30

KMMailComponent::KMMailComponent(QWidget *parent) :
    KMMailComponentBase(parent),
    m_titleBar(new KMMailComponentTitleBar(this)),
    m_content(new KMMailComponentContent(this))
{
    setObjectName("MailComponent");
    //Set properties.
    setAutoFillBackground(true);
    //Register the widget.
    knTheme->registerWidget(this);
    //Configure the title bar.
    connect(m_titleBar, &KMMailComponentTitleBar::titleSizeUpdate,
            this, &KMMailComponent::updateGeometries);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    //Set layout to container.
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_titleBar);
    //Add component widget to layout.
    mainLayout->addWidget(m_content, 1);

    //Debug.
    m_titleBar->setTitle("TechLauncher: First peer assessment is due at 17:00 today");
    m_titleBar->setReceiveDate(QDate(2016,3,1));
    //Debug
    QStringList receiverList;
    receiverList << "ayase.eri@ll-anime.com" << "tojo.nozomi@ll-anime.com";
    m_titleBar->setReceiverList(receiverList);
    receiverList << "yazawa.nico@ll-anime.com"
                 << "minami.kotori@ll-anime.com"
                 << "sonoda.umi@ll-anime.com"
                 << "nishikino.maki@ll-anime.com"
                 << "koitsumi.hanayo@ll-anime.com"
                 << "hoshizora.rin@ll-anime.com"
                 << "kousaka.honoka@ll-anime.com"
                 << "nitta.emi@ll-anime.com"
                 << "mimorin.suzuko@ll-anime.com"
                 << "utchida.aya@ll-anime.com"
                 << "kubo.yurika@ll-anime.com"
                 << "itta.riho@ll-anime.com"
                 << "pile@ll-anime.com"
                 << "toukui.sora@ll-anime.com"
                 << "nanjo.yoshino@ll-anime.com"
                 << "kusuda.aina@ll-anime.com";
    m_titleBar->setReceiverList(receiverList);
    QStringList senderList;
    senderList << "tojo.nozomi@ll-anime.com";
    m_titleBar->setSenderList(senderList);
}

void KMMailComponent::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KMMailComponentBase::resizeEvent(event);
    //Update the geometry.
    updateGeometries();
}

void KMMailComponent::updateGeometries()
{
    //Update the title bar width.
    m_titleBar->resize(width(), m_titleBar->height());
    //Update the content size.
    m_content->setGeometry(0,
                           m_titleBar->height(),
                           width(),
                           height()-m_titleBar->height());
}
