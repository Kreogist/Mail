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
#include <QTimeLine>

#include "kmcomposelineedit.h"

#define MaxAlpha 255
#define MoveAlpha 152
#define MinAlpha 50

KMComposeLineEdit::KMComposeLineEdit(QWidget *parent) :
    QLineEdit(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_currentAlpha(MinAlpha),
    m_isMouseIn(false)
{
    //Set properties.
    setFrame(false);
    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(33);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KMComposeLineEdit::onActionChangeAlpha);
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
    lineColor.setAlpha(m_currentAlpha);
    //Draw the bottom line.
    painter.setPen(lineColor);
    painter.drawLine(QPointF(0,height()), QPointF(width(), height()));
}

void KMComposeLineEdit::enterEvent(QEvent *event)
{
    //Do original event.
    QLineEdit::enterEvent(event);
    //Save the enter event.
    m_isMouseIn=true;
    //Check focus state.
    if(!hasFocus())
    {
        //Start to move anime.
        startAnime(MoveAlpha);
    }
}

void KMComposeLineEdit::leaveEvent(QEvent *event)
{
    //Do original event.
    QLineEdit::leaveEvent(event);
    //Save the leave event.
    m_isMouseIn=false;
    //Check focus state.
    if(!hasFocus())
    {
        //Start to move anime.
        startAnime(MinAlpha);
    }
}

void KMComposeLineEdit::focusInEvent(QFocusEvent *event)
{
    //Do original event.
    QLineEdit::focusInEvent(event);
    //Start the anime to max.
    startAnime(MaxAlpha);
}

void KMComposeLineEdit::focusOutEvent(QFocusEvent *event)
{
    //Do the original event.
    QLineEdit::focusOutEvent(event);
    //Start the anime to min if the mouse is not here.
    startAnime(m_isMouseIn?MoveAlpha:MinAlpha);
}

void KMComposeLineEdit::onActionChangeAlpha(int frame)
{
    //Update the alpha.
    m_currentAlpha=frame;
    //Update the widget.
    update();
}

inline void KMComposeLineEdit::startAnime(int endFrame)
{
    //Stop the anime.
    m_mouseAnime->stop();
    //Update the frame range.
    m_mouseAnime->setFrameRange(m_currentAlpha, endFrame);
    //Start the anime.
    m_mouseAnime->start();
}
