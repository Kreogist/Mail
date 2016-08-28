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
#include <QCursor>

#include "knunderlinelineedit.h"

#define BaseOpacity 150
#define HoverOpacity 204
#define SelectOpacity 255

KNUnderLineLineEdit::KNUnderLineLineEdit(QWidget *parent) :
    QLineEdit(parent),
    m_timeLine(new QTimeLine(200, this)),
    m_currentOpacity(BaseOpacity)
{
    //Set properties.
    setFrame(false);
    //Configure the time line.
    m_timeLine->setUpdateInterval(33);
    m_timeLine->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_timeLine, &QTimeLine::frameChanged,
            this, &KNUnderLineLineEdit::onActionUpdateOpacity);
    onActionUpdateOpacity(BaseOpacity);
}

void KNUnderLineLineEdit::updatePalette(const QPalette &pal)
{
    //Update the palette.
    setPalette(pal);
    //Update the opacity.
    onActionUpdateOpacity(m_currentOpacity);
}

void KNUnderLineLineEdit::enterEvent(QEvent *event)
{
    //Do original event.
    QLineEdit::enterEvent(event);
    //Check whether the widget has the focus.
    if(hasFocus())
    {
        //Then ignore the opacity animation.
        return;
    }
    //Start the animation.
    startAnime(HoverOpacity);
}

void KNUnderLineLineEdit::leaveEvent(QEvent *event)
{
    //Do original event.
    QLineEdit::leaveEvent(event);
    //Check whether the widget has the focus.
    if(hasFocus())
    {
        //Then ignore the opacity animation.
        return;
    }
    //Start the animation.
    startAnime(BaseOpacity);
}

void KNUnderLineLineEdit::focusInEvent(QFocusEvent *event)
{
    //Do original event.
    QLineEdit::focusInEvent(event);
    //Start the animation.
    startAnime(SelectOpacity);
}

void KNUnderLineLineEdit::focusOutEvent(QFocusEvent *event)
{
    //Do orignal event.
    QLineEdit::focusOutEvent(event);
    //Check the mouse position.
    startAnime((rect().contains(mapFromGlobal(QCursor::pos())))?
                   //Start the animation to hover.
                   HoverOpacity :
                   //Otherwise to base.
                   BaseOpacity);
}

void KNUnderLineLineEdit::paintEvent(QPaintEvent *event)
{
    //Draw the original data.
    QLineEdit::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing, true);
    //Draw the bottom line.
    int bottomHeight=height();
    painter.drawLine(QPoint(0, bottomHeight), QPoint(width(), bottomHeight));
}

void KNUnderLineLineEdit::onActionUpdateOpacity(int opacity)
{
    //Save the opacity.
    m_currentOpacity=opacity;
    //Get the palette.
    QPalette pal=palette();
    //Update the text color.
    QColor color=pal.color(QPalette::Text);
    color.setAlpha(opacity);
    pal.setColor(QPalette::Text, color);
    //Update the line color.
    color=pal.color(QPalette::Window);
    color.setAlpha(opacity>>1);
    pal.setColor(QPalette::Window, color);
    //Update the color.
    setPalette(pal);
}

inline void KNUnderLineLineEdit::startAnime(int targetOpacity)
{
    //Start the animation.
    m_timeLine->stop();
    //Update the frame range.
    m_timeLine->setFrameRange(m_currentOpacity, targetOpacity);
    //Start time line.
    m_timeLine->start();
}
