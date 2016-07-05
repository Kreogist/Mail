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

#include "knmainwindowleftbar.h"

KNMainWindowLeftBar::KNMainWindowLeftBar(QWidget *parent) :
    KNMainWindowLeftBarBase(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this))
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    //Configure the layout.
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

void KNMainWindowLeftBar::addLeftBarWidget(QWidget *widget,
                                           int stretch,
                                           Qt::Alignment alignment)
{
    //Add widget directly to the layout widget.
    m_mainLayout->addWidget(widget, stretch, alignment);
}
