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
#include <QTimeLine>
#include <QScrollBar>

#include "knthememanager.h"

#include "knmailfoldertreeview.h"

#define ScrollBarShow (150)
#define ScrollBarHide (0)

KNMailFolderTreeView::KNMailFolderTreeView(QWidget *parent) :
    KNMailTreeViewBase(parent),
    m_mouseAnime(new QTimeLine(200, this))
{
    setObjectName("MailFolderTreeView");
    //Set properties.
    knTheme->registerWidget(this);
    customScrollBar()->setObjectName("MailTreeViewScrollBar");
    //Link the theme changed signal.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMailFolderTreeView::onActionThemeChange);
    onActionThemeChange();

    //Configure the animation.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(33);
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KNMailFolderTreeView::onActionMouseInOut);
}

void KNMailFolderTreeView::enterEvent(QEvent *event)
{
    //Do original enter event.
    KNMailTreeViewBase::enterEvent(event);
    //Start the animation.
    startScrollAnime(ScrollBarShow);
}

void KNMailFolderTreeView::leaveEvent(QEvent *event)
{
    //Do original enter event.
    KNMailTreeViewBase::leaveEvent(event);
    //Start the animation.
    startScrollAnime(ScrollBarHide);
}

void KNMailFolderTreeView::onActionThemeChange()
{
    //Change the palette of the scroll bar.
    customScrollBar()->setPalette(
                knTheme->getPalette(customScrollBar()->objectName()));
    //Update the palette.
    onActionMouseInOut(0);
}

void KNMailFolderTreeView::onActionMouseInOut(int opacity)
{
    //Get the scroll bar palette.
    QPalette pal=customScrollBar()->palette();
    //Change the button color.
    QColor color=pal.color(QPalette::Button);
    color.setAlpha(opacity);
    pal.setColor(QPalette::Button, color);
    //Change the base color.
    color=pal.color(QPalette::Base);
    color.setAlpha(opacity>>1);
    pal.setColor(QPalette::Base, color);
    //Update the palette of the scroll bar.
    customScrollBar()->setPalette(pal);
}

inline void KNMailFolderTreeView::startScrollAnime(int targetAlpha)
{
    //Update the animation.
    m_mouseAnime->stop();
    //Update the frame range.
    m_mouseAnime->setFrameRange(
                customScrollBar()->palette().color(QPalette::Button).alpha(),
                targetAlpha);
    //Start the animation.
    m_mouseAnime->start();
}
