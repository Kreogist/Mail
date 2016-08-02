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
#include "knmailcontactcontainer.h"
#include "knmailcontactbutton.h"

#include "knmailcontactlist.h"

KNMailContactList::KNMailContactList(QWidget *parent) :
    QScrollArea(parent),
    m_container(new KNMailContactContainer(this))
{
    //Set properties.
    setFixedHeight(ButtonHeight); //Button height to the scroll area.
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidget(m_container);

    //Add container.
    m_container->addContact("tomguts@126.com", "Tojo Saki");
    m_container->addContact("freddiewanah@gmail.com", "Freddie");
}

void KNMailContactList::setContactPalette(const QPalette &pal)
{
    //Update the container and set the palette.
    m_container->setContactPalette(pal);
}

void KNMailContactList::resizeEvent(QResizeEvent *event)
{
    //Resize the area.
    QScrollArea::resizeEvent(event);
    //Resize the container, update the size.
    m_container->resize(width(),
                        m_container->heightForWidth(width()));
}
