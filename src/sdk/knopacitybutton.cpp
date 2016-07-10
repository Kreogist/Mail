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

#include "knopacitybutton.h"

#define OpacityGap 0.2
#define OpacityDefault 1.0

KNOpacityButton::KNOpacityButton(QWidget *parent) :
    QAbstractButton(parent),
    m_originalOpacity(OpacityDefault),
    m_opacity(OpacityDefault)
{
    //Set as strong focus.
    setFocusPolicy(Qt::StrongFocus);
    //Set toolbutton size policy.
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,
                              QSizePolicy::MinimumExpanding,
                              QSizePolicy::ToolButton));
}

void KNOpacityButton::paintEvent(QPaintEvent *event)
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
                               QPainter::TextAntialiasing, true);
    //Set opacity.
    painter.setOpacity(m_opacity);
    //Draw the pixmap.
    painter.drawPixmap(0,0,width(),height(),icon().pixmap(size()));
}

void KNOpacityButton::mousePressEvent(QMouseEvent *event)
{
    //Check is the button disabled.
    if(!isEnabled())
    {
        return;
    }
    //Do the original pressed event.
    QAbstractButton::mousePressEvent(event);
    //Set opacity.
    m_opacity=getPressedOpacity();
    //Update the widget.
    update();
}

void KNOpacityButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Check is the button disabled.
    if(!isEnabled())
    {
        return;
    }
    //Do the original released event.
    QAbstractButton::mouseReleaseEvent(event);
    //Recover opacity.
    m_opacity=m_originalOpacity;
    //Update the widget.
    update();
}

inline qreal KNOpacityButton::getPressedOpacity()
{
    //Set opacity.
    qreal opacity=OpacityDefault-OpacityGap;
    //If the opacity is less than 0, means this settings wants to increase the
    //opacity.
    return (opacity<0)?OpacityDefault+OpacityGap:opacity;
}

qreal KNOpacityButton::opacity() const
{
    return m_opacity;
}

void KNOpacityButton::setOpacity(const qreal &opacity)
{
    //Save the opacity as the default opacity.
    m_originalOpacity = opacity;
    //Reset the current opacity.
    m_opacity = m_originalOpacity;
    //Update the button.
    update();
}
