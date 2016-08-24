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
#include <QPainter>

#include "knmailrotatebutton.h"

KNMailRotateButton::KNMailRotateButton(QWidget *parent) :
    KNOpacityButton(parent),
    m_rotate(0)
{
}

void KNMailRotateButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    //Ignore painting request when disabled or the icon is null.
    if(icon().isNull() || !isEnabled())
    {
        return;
    }
    //Initial a painter.
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Set opacity.
    painter.setOpacity(opacity());
    //Apply the rotate.
    painter.translate(rect().center());
    painter.rotate(m_rotate);
    painter.translate(-width()>>1, -height()>>1);
    //Draw the pixmap.
    painter.drawPixmap(0,0,width(),height(),icon().pixmap(size()));
}

qreal KNMailRotateButton::rotate() const
{
    return m_rotate;
}

void KNMailRotateButton::setRotate(const qreal &rotate)
{
    //Save the rotateion.
    m_rotate = rotate;
    //Update the button.
    update();
}
