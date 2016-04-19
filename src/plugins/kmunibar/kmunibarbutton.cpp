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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QPainter>
#include <QTimeLine>

#include "kmunibarbutton.h"

#define ButtonHeight 38

KMUnibarButton::KMUnibarButton(QWidget *parent) :
    QAbstractButton(parent),
    m_anime(new QTimeLine(200, this)),
    m_opacity(0.0)
{
    //Set properties.
    setFixedHeight(ButtonHeight);
    //Configure the animation.
    m_anime->setEasingCurve(QEasingCurve::OutCubic);
    m_anime->setUpdateInterval(33);
    connect(m_anime, &QTimeLine::frameChanged,
            this, &KMUnibarButton::onActionChangeOpacity);
}

void KMUnibarButton::paintEvent(QPaintEvent *event)
{
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Draw the background.
    painter.setOpacity(m_opacity);
    painter.setBrush(palette().color(QPalette::Button));
    painter.drawRect(rect());
}

void KMUnibarButton::onActionChangeOpacity(int frame)
{
    ;
}
