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

#include "kmcomposelineedit.h"

KMComposeLineEdit::KMComposeLineEdit(QWidget *parent) :
    QLineEdit(parent),
    m_isEntered(false)
{
    //Set properties.
    setFrame(false);
}


void KMComposeLineEdit::paintEvent(QPaintEvent *event)
{
    //Draw the original stuffs.
    QLineEdit::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Get the color of current palette.
    QColor lineColor=palette().color(QPalette::Window);
    //Update the brightness when the mouse is out or the staff is not useful.
    lineColor.setAlpha((hasFocus() || m_isEntered)?255:100);
    //Draw the bottom line.
    painter.setPen(lineColor);
    painter.drawLine(QPointF(0,height()), QPointF(width(), height()));
}

void KMComposeLineEdit::enterEvent(QEvent *event)
{
    //Do original event.
    QLineEdit::enterEvent(event);
    //Update flag.
    m_isEntered=true;
    //Update the widget.
    update();
}

void KMComposeLineEdit::leaveEvent(QEvent *event)
{
    //Do original event.
    QLineEdit::leaveEvent(event);
    //Update flag.
    m_isEntered=false;
    //Update the widget.
    update();
}
