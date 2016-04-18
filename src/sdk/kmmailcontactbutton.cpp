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
#include "kmmailcontactlabelbutton.h"
#include "kmmailcontactaddbutton.h"

#include "kmmailcontactbutton.h"

KMMailContactButton::KMMailContactButton(QWidget *parent) :
    QWidget(parent),
    m_addButton(new KMMailContactAddButton(this)),
    m_labelButton(new KMMailContactLabelButton(this)),
    m_exist(false)
{
}

QSize KMMailContactButton::sizeHint() const
{
    //The label button size hint will be the contact button size hint.
    return QSize(m_labelButton->sizeHint().width() +
                 (m_exist?0:m_addButton->height()),
                 //Height is fixed.
                 m_labelButton->height());
}

QPixmap KMMailContactButton::avatarIcon() const
{
    return m_labelButton->avatarIcon();
}

void KMMailContactButton::setAvatarIcon(const QPixmap &avatarIcon)
{
    //Set the avatar.
    m_labelButton->setAvatarIcon(avatarIcon);
    //Update the size.
    setFixedSize(sizeHint());
}

void KMMailContactButton::setContactAddress(const QString &mailAddress)
{
    //Set the text data.
    m_labelButton->setText(mailAddress);
    //Update the size.
    setFixedSize(sizeHint());
}

void KMMailContactButton::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Move the add button.
    m_addButton->move(width()-m_addButton->width(), 0);
}

bool KMMailContactButton::exist() const
{
    return m_exist;
}

void KMMailContactButton::setExist(bool exist)
{
    //Save the exist state.
    m_exist = exist;
    //Update the size.
    setFixedSize(sizeHint());
}
