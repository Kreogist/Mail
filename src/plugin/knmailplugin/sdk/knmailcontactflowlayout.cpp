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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QWidget>

#include "knmailcontactflowlayout.h"

KNMailContactFlowLayout::KNMailContactFlowLayout(QWidget *parent) :
    QLayout(parent),
    m_hSpace(-1),
    m_vSpace(-1)
{
}

KNMailContactFlowLayout::KNMailContactFlowLayout(int hSpacing,
                                                 int vSpacing,
                                                 QWidget *parent) :
    QLayout(parent),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing)
{
}

KNMailContactFlowLayout::~KNMailContactFlowLayout()
{
    //Recover all the memory.
    QLayoutItem *item;
    //Get each item.
    while ((item = takeAt(0)))
    {
        //Remove the item.
        delete item;
    }
}

void KNMailContactFlowLayout::addItem(QLayoutItem *item)
{
    //Add item to the item list.
    m_itemList.append(item);
}

Qt::Orientations KNMailContactFlowLayout::expandingDirections() const
{
    //It is neither horizontal nor vertical.
    return 0;
}

int KNMailContactFlowLayout::heightForWidth(int width) const
{
    //Use the do layout to calculate the width.
    return doLayout(QRect(0, 0, width, 0), false);
}

void KNMailContactFlowLayout::setGeometry(const QRect &rect)
{
    //Set the layout of the geometry.
    QLayout::setGeometry(rect);
    //Apply the size to the layout.
    doLayout(rect, true);
}

QSize KNMailContactFlowLayout::sizeHint() const
{
    //Use the minimal size as the size hint.
    return minimumSize();
}

QSize KNMailContactFlowLayout::minimumSize() const
{
    //Initial a null size.
    QSize size;
    //Check all the item in the item list.
    foreach (QLayoutItem *item, m_itemList)
    {
        //Add the size to the item.
        size = size.expandedTo(item->minimumSize());
    }
    //Give back the size.
    return size;
}

QLayoutItem *KNMailContactFlowLayout::itemAt(int index) const
{
    //Use value to protect the environment.
    return m_itemList.value(index, nullptr);
}

QLayoutItem *KNMailContactFlowLayout::takeAt(int index)
{
    //Get the item if index is right in the bound.
    if(index>-1 && index<m_itemList.size())
    {
        //Get the item.
        return m_itemList.takeAt(index);
    }
    else
    {
        //A null pointer if the index is invalid.
        return nullptr;
    }
}

int KNMailContactFlowLayout::count() const
{
    //The size of the group could be the item.
    return m_itemList.size();
}

inline int KNMailContactFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    //Get the parent object.
    QObject *parentObject = parent();
    //Check the parent object pointer.
    if (!parentObject)
    {
        //No parent object, the spacing will be -1, it is an invalid value.
        return -1;
    }
    else if (parentObject->isWidgetType())
    {
        //Cast the parent object to widget.
        QWidget *parentWidget = static_cast<QWidget *>(parentObject);
        //Get the spacing of the widget via style.
        return parentWidget->style()->pixelMetric(pm, 0, parentWidget);
    }
    else
    {
        //It should be a layout, then get the the layout spacing.
        return static_cast<QLayout *>(parentObject)->spacing();
    }
}

inline int KNMailContactFlowLayout::doLayout(QRect effectiveRect,
                                             bool apply) const
{
    //Initial the rect parameters.
    int rectX=effectiveRect.x(),
        rectY=effectiveRect.y(),
        rectRight, lineHeight=0, spaceX, spaceY;
    //Initial the hidden flag.
    bool hiddenWidget=false;
    //Check out all the items.
    foreach(QLayoutItem *item, m_itemList)
    {
        //Get the widget from the item.
        QWidget *itemWidget=item->widget();
        //Check the item widget.
        if(itemWidget==nullptr)
        {
            //Goto next widget.
            continue;
        }
        //Reset the spacing X.
        spaceX=horizontalSpacing();
        //Check the spacing.
        if(spaceX==-1)
        {
            //Get the style layout spacing.
            spaceX=itemWidget->style()->layoutSpacing(QSizePolicy::PushButton,
                                                      QSizePolicy::PushButton,
                                                      Qt::Horizontal);
        }
        //Reset the spacing Y.
        spaceY=verticalSpacing();
        //Check the spacing.
        if(spaceY==-1)
        {
            //Get the style layout spacing.
            spaceY=itemWidget->style()->layoutSpacing(QSizePolicy::PushButton,
                                                      QSizePolicy::PushButton,
                                                      Qt::Vertical);
        }
        //Calculate the right position.
        rectRight=rectX+item->sizeHint().width();
        //Check whether the item is still in the border.
        if(rectRight > effectiveRect.right() && lineHeight > 0)
        {
            //Reset the horizontal position.
            rectX=effectiveRect.x();
            //Move vertical position to next line.
            rectY+=(lineHeight + spaceY);
            //Reset the rect right,
            rectRight=rectX+item->sizeHint().width();
            //Reset the line height.
            lineHeight=0;
        }
        //If this function is set to apply the change, then move the item.
        if(apply)
        {
            //Move the item
            item->setGeometry(QRect(QPoint(rectX, rectY), item->sizeHint()));
        }
        //Update the rect x.
        rectX=rectRight+spaceX;
        //Update the line height.
        lineHeight=qMax(lineHeight, item->sizeHint().height());
    }
    //Give back the size which is out side the layout.
    return rectY + lineHeight - rect.y();
}
