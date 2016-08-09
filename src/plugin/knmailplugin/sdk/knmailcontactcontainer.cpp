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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>

#include "knmailcontactbutton.h"
#include "knmailcontactflowlayout.h"

#include "knmailcontactcontainer.h"

KNMailContactContainer::KNMailContactContainer(QWidget *parent) :
    QWidget(parent),
    m_mainLayout(new KNMailContactFlowLayout(LayoutSpacing,
                                             LayoutSpacing,
                                             this))
{
    //Set main layout.
    setLayout(m_mainLayout);
}

void KNMailContactContainer::addContact(KNMailContactButton *button)
{
    //Add button to main layout.
    m_mainLayout->addWidget(button);
    //Add button to list.
    m_buttons.append(button);
}

void KNMailContactContainer::addContact(const QString &email,
                                        const QString &caption)
{
    //Construct the button.
    KNMailContactButton *contactButton=new KNMailContactButton(this);
    //Set the settings.
    contactButton->setContact(caption, email);
    //Add contact to the list.
    addContact(contactButton);
}

bool KNMailContactContainer::hasHeightForWidth() const
{
    //Has the height by the devided width.
    return true;
}

int KNMailContactContainer::heightForWidth(int targetWidth) const
{
    //Get the height.
    return m_mainLayout->heightForWidth(targetWidth);
}

void KNMailContactContainer::setContactPalette(const QPalette &pal)
{
    //Update the button in the list.
    for(auto i : m_buttons)
    {
        //Update the button.
        i->setPalette(pal);
    }
}

void KNMailContactContainer::resizeEvent(QResizeEvent *event)
{
    //Update the size.
    QWidget::resizeEvent(event);
    //Update the button sizes.
    for(auto &&i : m_buttons)
    {
        //Update the button size.
        i->updateWidth();
    }
}
