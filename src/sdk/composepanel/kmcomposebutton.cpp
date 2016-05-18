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

#include "kmcomposebutton.h"

#define RoundedRadius 3
#define MaxAlpha 160
#define HoverAlpha 100
#define PressedAlpha 100
#define MinAlpha 0

KMComposeButton::KMComposeButton(QWidget *parent) :
    QAbstractButton(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_currentAlpha(MinAlpha)
{
    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(33);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KMComposeButton::onActionChangeAlpha);
}

void KMComposeButton::setChecked(bool checked)
{
    //Update the button.
    QAbstractButton::setChecked(checked);
    //Change the alpha.
    m_currentAlpha=checked?HoverAlpha:MinAlpha;
    //Update the button.
    update();
}

void KMComposeButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    //Configure the painter.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the background of the button.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().brush(QPalette::Button));
    painter.setOpacity(m_currentAlpha/255.0);
    painter.drawRoundedRect(rect(), RoundedRadius, RoundedRadius);
    //Reset the opacity.
    painter.setOpacity(1.0);
    //Draw the content of the button.
    //Check if the button doesn't contain text, then treat as image button.
    if(text().isEmpty())
    {
        //Get the pixmap.
        QPixmap &drawnIcon=icon().pixmap(size());
        //Draw the button at the central of the button.
        painter.drawPixmap((width()-drawnIcon.width())>>1,
                           (height()-drawnIcon.height())>>1,
                           drawnIcon);
        //Complete.
        return;
    }
    //Draw the button text.
    painter.drawText(rect(), Qt::AlignCenter, text());
}

void KMComposeButton::enterEvent(QEvent *event)
{
    //Do original event.
    QAbstractButton::enterEvent(event);
    //Start to move anime.
    startAnime(HoverAlpha);
}


void KMComposeButton::leaveEvent(QEvent *event)
{
    //Do original event.
    QAbstractButton::leaveEvent(event);
    //Start to move anime.
    startAnime(isChecked()?PressedAlpha:MinAlpha);
}

void KMComposeButton::focusInEvent(QFocusEvent *event)
{
    //Do original event.
    QAbstractButton::focusInEvent(event);
    //Start the anime to max.
    startAnime(HoverAlpha);
}

void KMComposeButton::focusOutEvent(QFocusEvent *event)
{
    //Do the original event.
    QAbstractButton::focusOutEvent(event);
    //Start the anime to min if the mouse is not here.
    startAnime(isChecked()?HoverAlpha:MinAlpha);
}

void KMComposeButton::mousePressEvent(QMouseEvent *event)
{
    //Do the original event.
    QAbstractButton::mousePressEvent(event);
    //Start the anime to max.
    startAnime(MaxAlpha);
}

void KMComposeButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the original event.
    QAbstractButton::mouseReleaseEvent(event);
    //Start the anime to hover.
    startAnime(HoverAlpha);
}

void KMComposeButton::onActionChangeAlpha(int frame)
{
    //Update the alpha.
    m_currentAlpha=frame;
    //Update the widget.
    update();
}

inline void KMComposeButton::startAnime(int endFrame)
{
    //Stop the anime.
    m_mouseAnime->stop();
    //Update the frame range.
    m_mouseAnime->setFrameRange(m_currentAlpha, endFrame);
    //Start the anime.
    m_mouseAnime->start();
}
