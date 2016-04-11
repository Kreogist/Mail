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

#include "kmmailcontactaddbutton.h"

#define ButtonRoundedRadius 4
#define OffsetData 10
#define ContactButtonHeight 27

KMMailContactAddButton::KMMailContactAddButton(QWidget *parent) :
    QAbstractButton(parent)
{
    //Set properties.
    setFixedSize(ContactButtonHeight + OffsetData,
                 ContactButtonHeight);
    //Update the font.
    QFont buttonFont=font();
    buttonFont.setPixelSize((ContactButtonHeight>>2)*3);
    setFont(buttonFont);
}

void KMMailContactAddButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the rounded rect.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().color(QPalette::Window));
    painter.drawRoundedRect(QRect(0,
                                  0,
                                  ContactButtonHeight+OffsetData,
                                  ContactButtonHeight),
                            ButtonRoundedRadius,
                            ButtonRoundedRadius);
    //Draw the plus button.
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawText(QRect(OffsetData,
                           0,
                           ContactButtonHeight,
                           ContactButtonHeight),
                     Qt::AlignCenter,
                     "+");
}
