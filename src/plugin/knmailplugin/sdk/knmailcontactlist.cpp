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
#include <QResizeEvent>
#include <QScrollBar>

#include "knroundedborderbutton.h"

#include "knmailcontactcontainer.h"
#include "knmailcontactbutton.h"

#include "knmailcontactlist.h"

#include <QDebug>

#define MaxListHeight ((ButtonHeight<<2) + (LayoutSpacing<<1))
#define RightMargin 25

KNMailContactList::KNMailContactList(QWidget *parent) :
    QScrollArea(parent),
    m_container(new KNMailContactContainer(this)),
    m_moreButton(new KNRoundedBorderButton(this)),
    m_isExpand(false)
{
    //Set properties.
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumHeight(ButtonHeight);
    setMaximumHeight(ButtonHeight);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidget(m_container);
    //Configure the button.
    m_moreButton->setFixedSize(ButtonHeight, ButtonHeight);
    m_moreButton->hide();
    connect(m_moreButton, &KNRoundedBorderButton::clicked,
            [=]{setExpandState(!m_isExpand);});
    //Update more button.
    updateExpandButton();
}

void KNMailContactList::addContact(KNMailContactButton *button)
{
    //Add the container data to the list.
    m_container->addContact(button);
}

void KNMailContactList::addContact(const QString &email, const QString &caption)
{
    //Add the data to contianer.
    m_container->addContact(email, caption);
}

void KNMailContactList::setContactPalette(const QPalette &pal)
{
    //Update the container and set the palette.
    m_container->setContactPalette(pal);
    //Update the more button as well.
    m_moreButton->setPalette(pal);
}

void KNMailContactList::resizeEvent(QResizeEvent *event)
{
    //Get the target height.
    int containerWidth=event->size().width()-RightMargin,
            targetHeight=m_container->heightForWidth(containerWidth);
    //Check out the size of the container height.
    //Then we have to make the expand button visible when has a larger size.
    m_moreButton->setVisible(targetHeight > ButtonHeight);
    //Check the state of the more button.
    if(!m_moreButton->isVisible() && m_isExpand)
    {
        //Reset the expand state.
        m_isExpand=false;
        //Set maximum height.
        setMaximumHeight(ButtonHeight);
        //Reset the position.
        verticalScrollBar()->setValue(0);
    }
    //Update the more button state.
    updateExpandButton();
    //Resize the container, update the size.
    m_container->resize(containerWidth,
                        m_container->heightForWidth(containerWidth));
    //Get the new height.
    targetHeight=m_container->height();
    //Check the expand state.
    if(!m_isExpand)
    {
        //Update the widget height to only one line.
        QResizeEvent listResizeEvent(size(),
                                     QSize(containerWidth, ButtonHeight));
        //Resize the area.
        QScrollArea::resizeEvent(&listResizeEvent);
        return;
    }
    //We could now check the height of the container.
    if(targetHeight > MaxListHeight)
    {
        //Update the widget height.
        QResizeEvent listResizeEvent(size(), QSize(event->size().width(),
                                                   MaxListHeight));
        //Resize the area.
        QScrollArea::resizeEvent(&listResizeEvent);
        //Show the custom scroll bar.
        ;
    }
    else
    {
        //Update the widget height.
        QResizeEvent listResizeEvent(size(),
                                     QSize(containerWidth, targetHeight));
        //Resize the area.
        QScrollArea::resizeEvent(&listResizeEvent);
        //Hide the custom scroll bar.
        ;
    }
}

void KNMailContactList::showEvent(QShowEvent *event)
{
    //Show the widget.
    QScrollArea::showEvent(event);
    //Check item height.
    m_moreButton->move(width()-m_moreButton->width(), 0);
}

void KNMailContactList::updateExpandButton()
{
    //Check the expand state.
    if(m_isExpand)
    {
        //Update the widget.
        m_moreButton->setText(" ^ ");
    }
    else
    {
        //Process the fold state.
        m_moreButton->setText("...");
    }
    //Update the more button position.
    m_moreButton->move(width()-m_moreButton->width(), 0);
}

void KNMailContactList::setExpandState(bool isExpand)
{
    //Check the exapnd state.
    if(m_moreButton->isHidden() || m_isExpand==isExpand)
    {
        //Ignore the state.
        return;
    }
    //Save the expand state.
    m_isExpand=isExpand;
    //Check the state height.
    if(m_isExpand)
    {
        //Set maximum height.
        setMaximumHeight(MaxListHeight);
        //Expand the widget.
        resize(width(), qMin(m_container->height(), MaxListHeight));
    }
    else
    {
        //Set maximum height.
        setMaximumHeight(ButtonHeight);
        //Fold the widget.
        resize(width(), ButtonHeight);
        //Reset the position.
        verticalScrollBar()->setValue(0);
    }
    //Update button.
    updateExpandButton();
}

void KNMailContactList::clear()
{
    m_container->clear();
}
