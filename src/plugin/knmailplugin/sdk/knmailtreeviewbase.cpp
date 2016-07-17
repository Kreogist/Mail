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
#include <QScrollBar>

#include "knmousesenseheader.h"
#include "sao/knsaostyle.h"

#include "knmailtreeviewbase.h"

#define ScrollBarWidth (10)

KNMailTreeViewBase::KNMailTreeViewBase(QWidget *parent) :
    QTreeView(parent),
    m_scrollBar(new QScrollBar(this))
{
    //Set properties.
    setAllColumnsShowFocus(true);
    setContentsMargins(0, 0, 0, 0);
    setFrameShape(QFrame::NoFrame);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Generate the sense tree view header.
    KNMouseSenseHeader *header=new KNMouseSenseHeader(this);
    //Set the header.
    setHeader(header);

    //Configure the vertical scroll bar.
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->setFixedWidth(ScrollBarWidth);
    m_scrollBar->hide();
    //Link the vertical scroll bar to custom scroll bar.
    connect(verticalScrollBar(), &QScrollBar::rangeChanged,
            this, &KNMailTreeViewBase::onActionRangeChanged);
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &KNMailTreeViewBase::onActionValueChanged);
    //Link the custom scroll bar to the original.
    connect(m_scrollBar, &QScrollBar::valueChanged,
            verticalScrollBar(), &QScrollBar::setValue);
}

void KNMailTreeViewBase::resizeEvent(QResizeEvent *event)
{
    //Resize the tree view.
    QTreeView::resizeEvent(event);
    //Update the position of the scroll bar.
    updateScrollBarGeometry();
}

void KNMailTreeViewBase::onActionRangeChanged(int min, int max)
{
    //Update the range first.
    m_scrollBar->setRange(min, max);
    //Update the visibility
    m_scrollBar->setVisible(min!=max);
    //Update scroll bar state parameter.
    m_scrollBar->setPageStep(verticalScrollBar()->pageStep());
    m_scrollBar->setSingleStep(verticalScrollBar()->singleStep());
    //Update the geometry of scroll bar.
    updateScrollBarGeometry();
}

void KNMailTreeViewBase::onActionValueChanged(int value)
{
    //Block the signal.
    m_scrollBar->blockSignals(true);
    //Update the value.
    m_scrollBar->setValue(value);
    //Release blocking.
    m_scrollBar->blockSignals(false);
}

inline void KNMailTreeViewBase::updateScrollBarGeometry()
{
    //Update the scroll bar, check the horizontal scroll bar is visible or not.
    int hScrollBarHeight=
            (horizontalScrollBar()!=nullptr &&
            horizontalScrollBar()->isVisible()) ? ScrollBarWidth : 0;
    //Update the scroll bar geometry.
    m_scrollBar->setGeometry(width()-ScrollBarWidth,
                             header()->height(),
                             ScrollBarWidth,
                             height()-hScrollBarHeight-header()->height());
}

QScrollBar *KNMailTreeViewBase::customScrollBar()
{
    return m_scrollBar;
}
