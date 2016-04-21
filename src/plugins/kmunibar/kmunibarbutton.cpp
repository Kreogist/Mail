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

#include "knthememanager.h"

#include "kmunibarbutton.h"

#define ButtonHeight 38

KMUnibarButton::KMUnibarButton(QWidget *parent) :
    QAbstractButton(parent),
    m_hoverOpacity(0.0),
    m_opacity(0.0)
{
    setObjectName("UnibarButton");
    //Set properties.
    setFixedHeight(ButtonHeight);
    //Register widget.
    knTheme->registerWidget(this);
}

void KMUnibarButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform,
                           true);
    //Draw the background, check the enabled.
    if(isEnabled())
    {
        //Check whether the button is checked.
        if(isChecked())
        {
            //Fill the rect with the button brush.
            painter.fillRect(rect(), palette().button());
        }
        else
        {
            painter.setOpacity(m_hoverOpacity);
            painter.setPen(Qt::NoPen);
            painter.setBrush(palette().color(QPalette::Button));
            painter.drawRect(rect());
        }
    }
    //Draw the text.
    painter.setOpacity(m_opacity);
    painter.setPen(palette().color(QPalette::ButtonText));
    painter.drawText(rect(), Qt::AlignVCenter | Qt::AlignLeft, text());
}

void KMUnibarButton::enterEvent(QEvent *event)
{
    //Do original event.
    QWidget::enterEvent(event);
    //Check enabled or not.
    if(isEnabled())
    {
        //Change the opacity.
        m_hoverOpacity=0.9;
        //Update the widget.
        update();
    }
}

void KMUnibarButton::leaveEvent(QEvent *event)
{
    //Do original event.
    QWidget::leaveEvent(event);
    //Checn enabled or not.
    if(isEnabled())
    {
        //Change the opacity.
        m_hoverOpacity=0.0;
        //Update the widget.
        update();
    }
}

qreal KMUnibarButton::opacity() const
{
    return m_opacity;
}

void KMUnibarButton::setOpacity(const qreal &opacity)
{
    //Update the opacity.
    m_opacity = opacity;
    //Update widget.
    update();
}
