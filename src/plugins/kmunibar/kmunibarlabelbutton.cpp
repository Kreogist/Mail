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

#include "kmunibarlabelbutton.h"

KMUnibarLabelButton::KMUnibarLabelButton(QWidget *parent) :
    QWidget(parent),
    m_image(QPixmap()),
    m_rotate(0)
{
    //Set properties.
    setCursor(Qt::PointingHandCursor);
}

int KMUnibarLabelButton::rotate() const
{
    return m_rotate;
}

void KMUnibarLabelButton::setRotate(int rotate)
{
    m_rotate = rotate;
}

void KMUnibarLabelButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Check the is null or not.
    if(m_image.isNull())
    {
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the pixmap at center.
    painter.translate(rect().center());
    //Do rotate.
    painter.rotate(m_rotate);
    //Update the position.
    painter.translate(-m_image.width()>>1,
                      -m_image.height()>>1);
    //Draw the image.
    painter.drawPixmap(0, 0, m_image);
}

void KMUnibarLabelButton::mousePressEvent(QMouseEvent *event)
{
    //Do origianl event.
    QWidget::mousePressEvent(event);
    //Emit clicked signal.
    emit clicked();
}

QPixmap KMUnibarLabelButton::image() const
{
    return m_image;
}

void KMUnibarLabelButton::setImage(const QPixmap &image)
{
    m_image = image;
}
