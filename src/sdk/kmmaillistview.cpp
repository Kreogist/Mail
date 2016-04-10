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
#include <QTimeLine>

#include "knthememanager.h"

#include "kmmaillistviewdelegate.h"

#include "kmmaillistview.h"

#define MaxOpacity 0x10
#define FontBase 0xBF

KMMailListView::KMMailListView(QWidget *parent) :
    QListView(parent),
    m_mouseAnime(new QTimeLine(200, this))
{
    //Set properties.
    setAutoFillBackground(true);
    setItemDelegate(new KMMailListViewDelegate(this));
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
//    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(10);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KMMailListView::onActionMouseInOut);

    //Link with theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KMMailListView::onActionThemeUpdate);
}

void KMMailListView::updateObjectName(const QString &name)
{
    //Set the object name.
    setObjectName(name);
    //Update the frame.
    onActionThemeUpdate();
}

void KMMailListView::enterEvent(QEvent *event)
{
    //Enter the list view.
    QListView::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KMMailListView::leaveEvent(QEvent *event)
{
    //Leave the list view.
    QListView::leaveEvent(event);
    //Start mouse leave anime.
    startAnime(0);
}

void KMMailListView::onActionThemeUpdate()
{
    //Get the new palette from theme manager, and set it.
    setPalette(knTheme->getPalette(objectName()));
    //Update the palette.
    onActionMouseInOut(0);
}

void KMMailListView::onActionMouseInOut(int frame)
{
    //Get the palette.
    QPalette pal=palette();
    //Update the text palette color.
    QColor color=pal.color(QPalette::Text);
    color.setAlpha(FontBase+(frame<<1));
    pal.setColor(QPalette::Text, color);
    //Update the alternating base color.
    color=pal.color(QPalette::AlternateBase);
    color.setAlpha(frame);
    pal.setColor(QPalette::AlternateBase, color);
    //Update the button color.
    color=pal.color(QPalette::Button);
    color.setHsv(color.hue(), color.saturation(), frame<<1);
    pal.setColor(QPalette::Button, color);
    //Set the palette.
    setPalette(pal);
}

inline void KMMailListView::startAnime(int endFrame)
{
    //Stop the mouse animations.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(
                palette().color(QPalette::AlternateBase).alpha(),
                endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
