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

#include "sao/knsaostyle.h"

#include "knsensescrollarea.h"

KNSenseScrollArea::KNSenseScrollArea(QWidget *parent) :
    QScrollArea(parent),
    m_scrollBar(new QScrollBar(this))
{
    //Configure the vertical scroll bar.
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->setFixedWidth(ScrollBarWidth);
    m_scrollBar->hide();
    //Link the vertical scroll bar to custom scroll bar.
    connect(verticalScrollBar(), &QScrollBar::rangeChanged,
            this, &KNSenseScrollArea::onActionRangeChanged);
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            this, &KNSenseScrollArea::onActionValueChanged);
    //Link the custom scroll bar to the original.
    connect(m_scrollBar, &QScrollBar::valueChanged,
            verticalScrollBar(), &QScrollBar::setValue);
}

void KNSenseScrollArea::onActionRangeChanged(int min, int max)
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

void KNSenseScrollArea::onActionValueChanged(int value)
{
    //Block the signal.
    m_scrollBar->blockSignals(true);
    //Update the value.
    m_scrollBar->setValue(value);
    //Release blocking.
    m_scrollBar->blockSignals(false);
}

inline void KNSenseScrollArea::updateScrollBarGeometry()
{
    ;
}

QScrollBar *KNSenseScrollArea::customScrollBar()
{
    return m_scrollBar;
}

void KNSenseScrollArea::resizeEvent(QResizeEvent *event)
{
    //Update the widget size.
    QScrollArea::resizeEvent(event);
    //Update the scroll bar, check the horizontal scroll bar is visible or not.
    int hScrollBarHeight=
            (horizontalScrollBar()!=nullptr &&
            horizontalScrollBar()->isVisible()) ? ScrollBarWidth : 0;
    //Update the scroll bar height.
    m_scrollBar->setGeometry(width()-ScrollBarWidth,
                             0,
                             height()-hScrollBarHeight,
                             ScrollBarWidth);
}
