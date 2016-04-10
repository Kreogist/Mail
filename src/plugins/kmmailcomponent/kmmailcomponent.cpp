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

#include "knthememanager.h"

#include "kmmailcomponenttitlebar.h"
#include "kmmailcomponentcontent.h"

#include "kmmailcomponent.h"

#include <QDebug>

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

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_titleBar);
    //Add component widget to layout.
    mainLayout->addWidget(m_content, 1);

    //Debug.
    m_titleBar->setTitle("TechLauncher: First peer assessment is due at 17:00 today");
    m_titleBar->setReceiveDate(QDate(2016,3,1));
}
