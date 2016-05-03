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
#include <QScrollBar>
#include <QTimeLine>

#include "sao/knsaostyle.h"
#include "knthememanager.h"
#include "kmmailcomponentcontactlist.h"

#include "kmmailcomponentcontactarea.h"

#include <QDebug>

#define LineHeight 27
#define LineSpacing 3
#define ExpandedLineCount 4
#define ScrollBarWidth 10
#define MaxOpacity 0x30

KMMailComponentContactArea::KMMailComponentContactArea(QWidget *parent) :
    QScrollArea(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_scrollBar(new QScrollBar(this)),
    m_contactList(nullptr)
{
    //Set properties.
    setFrameStyle(QFrame::NoFrame);
    //Configure the scroll bar policies.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //Initial the scroll bar.
    //Configure the scroll bar.
    m_scrollBar->hide();
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->setSingleStep(27);
    m_scrollBar->setPageStep(27);
    m_scrollBar->setObjectName("MailComponentScrollBar");
    connect(knTheme, &KNThemeManager::themeChange,
            [=]
            {
                //Get the new palette from theme manager, and set it.
                m_scrollBar->setPalette(knTheme->getPalette(
                                            m_scrollBar->objectName()));
                //Update the palette.
                onActionMouseInOut(0);
            });
    //Update the validation.
    m_scrollBar->setPalette(knTheme->getPalette(m_scrollBar->objectName()));
    onActionMouseInOut(0);

    //Update the single step.
    verticalScrollBar()->setSingleStep(27);

    connect(verticalScrollBar(), &QScrollBar::rangeChanged,
            this, &KMMailComponentContactArea::onActionRangeChange);
    connect(verticalScrollBar(), &QScrollBar::valueChanged,
            [=](int value)
            {
                //Block the signal.
                m_scrollBar->blockSignals(true);
                //Reset the value.
                m_scrollBar->setValue(value);
                //Release the block
                m_scrollBar->blockSignals(false);
            });
    connect(m_scrollBar, &QScrollBar::valueChanged,
            verticalScrollBar(), &QScrollBar::setValue);

    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(10);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KMMailComponentContactArea::onActionMouseInOut);
}

void KMMailComponentContactArea::setContactList(
        KMMailComponentContactList *list)
{
    //Check the pointer before.
    if(m_contactList)
    {
        //Disconnect the previous signals.
        disconnect(m_contactList, 0, 0, 0);
    }
    //Save the list.
    m_contactList=list;
    //Check the pointer.
    if(!m_contactList)
    {
        //For null pointer, ignore the following operations.
        return;
    }
    //Set the contacat list to be the managed widget.
    setWidget(m_contactList);
    //Link the list widget.
    connect(m_contactList, &KMMailComponentContactList::expandStateChange,
            this, &KMMailComponentContactArea::onExpandStateChange);
    connect(m_contactList, &KMMailComponentContactList::lineCountChanged,
            this, &KMMailComponentContactArea::onExpandStateChange);
}

void KMMailComponentContactArea::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QScrollArea::resizeEvent(event);
    //Update geometries.
    updateGeometries();
}

void KMMailComponentContactArea::enterEvent(QEvent *event)
{
    //Enter the widget.
    QScrollArea::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KMMailComponentContactArea::leaveEvent(QEvent *event)
{
    //Enter the widget.
    QScrollArea::leaveEvent(event);
    //Start mouse in anime.
    startAnime(0);
}

void KMMailComponentContactArea::onActionMouseInOut(int frame)
{
    //Update the scroll bar color.
    QPalette pal=m_scrollBar->palette();
    QColor color=pal.color(QPalette::Base);
    color.setAlpha(frame);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(frame<<1);
    pal.setColor(QPalette::Button, color);
    //Set the palette to scroll bar.
    m_scrollBar->setPalette(pal);
}

void KMMailComponentContactArea::onActionRangeChange(int min, int max)
{
    //Update the range first.
    m_scrollBar->setRange(min, max);
    //Check whether the scroll bar is still valid.
    m_scrollBar->setVisible(min!=max);
}

void KMMailComponentContactArea::onExpandStateChange()
{
    //Check the contact list pointer.
    if(!m_contactList)
    {
        //Ignore the null pointer.
        return;
    }
    //Check the expanded state.
    if(m_contactList->isExpaned())
    {
        //Change the height of the area to maximum 5 line's height.
        setFixedHeight(qMin(LineHeight * ExpandedLineCount +
                            (LineSpacing) * (ExpandedLineCount - 1),
                            m_contactList->height()));
    }
    else
    {
        //Chagne the height to be a single line.
        setFixedHeight(LineHeight);
    }
    //Emit the size changed signal.
    emit areaHeightChange();
}

void KMMailComponentContactArea::updateGeometries()
{
    //Update the scroll bar position.
    m_scrollBar->setGeometry(width()-ScrollBarWidth,
                             0,
                             ScrollBarWidth,
                             height());
    //Configure the width of the contact list.
    if(m_contactList)
    {
        //Resize the contact list.
        m_contactList->resize(width(), m_contactList->height());
    }
}

void KMMailComponentContactArea::startAnime(int endFrame)
{
    //Stop the mouse animations.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(
                m_scrollBar->palette().color(QPalette::Base).alpha(),
                endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
