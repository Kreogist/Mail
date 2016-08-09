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

#include <QPainter>

#include "knroundedborderbutton.h"

#define ButtonBorderRadius 4

KNRoundedBorderButton::KNRoundedBorderButton(QWidget *parent) :
    QAbstractButton(parent)
{
}

void KNRoundedBorderButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Update the border.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().color(QPalette::Button));
    //Draw the background.
    painter.drawRoundedRect(rect(), ButtonBorderRadius, ButtonBorderRadius);
    //Draw the button text.
    painter.setPen(palette().color(QPalette::ButtonText));
    painter.drawText(rect(), Qt::AlignCenter, text());
}
