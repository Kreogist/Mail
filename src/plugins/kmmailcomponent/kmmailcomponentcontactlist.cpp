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
#include <QBoxLayout>

#include "kmextendbutton.h"

#include "kmmailcomponentcontactlist.h"

#include <QDebug>

KMMailComponentContactList::KMMailComponentContactList(QWidget *parent) :
    QWidget(parent),
    m_listLayout(new QBoxLayout(QBoxLayout::TopToBottom)),
    m_extendButton(new KMExtendButton(this)),
    m_fold(false),
    m_isFold(true)
{
    //Configure the extended button.
    m_extendButton->hide();
    connect(m_extendButton, &KMExtendButton::clicked,
            this, &KMMailComponentContactList::onActionChangeExpand);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(3);
    setLayout(mainLayout);
    //Add layout to main layout.
    mainLayout->addLayout(m_listLayout);
    mainLayout->addWidget(m_extendButton, 1, Qt::AlignBottom | Qt::AlignLeft);
}

void KMMailComponentContactList::addWidget(QWidget *widget)
{
    //Add widget to list layout.
    m_listLayout->addWidget(widget, 0, Qt::AlignLeft | Qt::AlignTop);
    //Add widget to list.
    m_buttonList.append(widget);
    //Check whether the widget is the first widget.
    if(m_fold && m_buttonList.size()>1)
    {
        //Show the button.
        m_extendButton->show();
        //Update the widget visiblility.
        widget->setVisible(!m_isFold);
    }
    //Check the size.
    if(m_fold && (!m_isFold))
    {
        //Update the list size.
        updateExpandedSize();
    }
}

void KMMailComponentContactList::clear()
{
    //Clear the button list.
    m_buttonList.clear();
    //Reset the folding state.
    m_isFold=true;
    //Hide the folded button.
    m_extendButton->hide();
    //Resize the height.
    setFixedHeight(27);
}

bool KMMailComponentContactList::isExpaned() const
{
    return m_fold && (!m_isFold);
}

void KMMailComponentContactList::setEnableFold(bool enableFold)
{
    //Save the value.
    m_fold=enableFold;
    //Check the states.
    if(m_fold)
    {
        //Reset the folding state.
        m_isFold=true;
    }
    //Update the button.
    m_extendButton->setVisible(m_fold && (m_buttonList.size()>1));
    //Update the layout.
    updateLayout();
}

void KMMailComponentContactList::onActionChangeExpand()
{
    //Update the expand state.
    m_isFold=!m_isFold;
    //Update layout.
    updateLayout();
}

inline void KMMailComponentContactList::updateLayout()
{
    //Check whether the folding is enabled.
    if(m_fold)
    {
        //Update the widget visible.
        for(int i=1; i<m_buttonList.size(); ++i)
        {
            //Set the visible to be the same as opposite is folding state.
            m_buttonList.at(i)->setVisible(!m_isFold);
        }
        //Update the list size.
        if(m_isFold)
        {
            //Update the size.
            setFixedHeight(27);
        }
        else
        {
            //Update the list size.
            updateExpandedSize();
        }
        //Update the widget.
        update();
        //Emit state changed signal.
        emit expandStateChange();
        //Mission complete.
        return;
    }
    //Update the widget visible.
    for(int i=1; i<m_buttonList.size(); ++i)
    {
        //Set the visible to be the same as opposite is folding state.
        m_buttonList.at(i)->setVisible(true);
    }
    //Update the list size.
    updateExpandedSize();
    //Update the widget.
    update();
    //Emit state changed signal.
    emit expandStateChange();
}

inline void KMMailComponentContactList::updateExpandedSize()
{
    //Update the size.
    setFixedHeight(m_buttonList.size()*27+(m_buttonList.size()-1)*3);
}
