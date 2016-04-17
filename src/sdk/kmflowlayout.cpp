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
#include <QWidget>

#include "kmextendbutton.h"

#include "kmflowlayout.h"

KMFlowLayout::KMFlowLayout(int margin, int hSpacing, int vSpacing,
                           QWidget *parent) :
    QLayout(parent),
    m_extendButton(new KMExtendButton(parent)),
    m_hSpace(hSpacing),
    m_vSpace(vSpacing),
    m_fold(false),
    m_isFolded(false)
{
    //Hide the extend button as default.
    m_extendButton->hide();
    //Update the margin.
    setContentsMargins(margin, margin, margin, margin);
}

KMFlowLayout::~KMFlowLayout()
{
    //Clear all the items.
    clearItems();
}

void KMFlowLayout::addItem(QLayoutItem *item)
{
    //Add the item to item list.
    itemList.append(item);
}

int KMFlowLayout::horizontalSpacing() const
{
    //Check the user setting spacing.
    return (m_hSpace >= 0) ?
                //Give back the space.
                m_hSpace:
                //Calculate the spacing.
                smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int KMFlowLayout::verticalSpacing() const
{
    //Check the user setting spacing.
    return (m_vSpace >= 0) ?
                //Give back the spacing.
                m_vSpace :
                //Calculate the spacing.
                smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int KMFlowLayout::count() const
{
    //Give back the item list size.
    return itemList.size();
}

QLayoutItem *KMFlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *KMFlowLayout::takeAt(int index)
{
    //Check the index size.
    return (index >= 0 && index < itemList.size())?
                //If the index is valid, give back the item.
                itemList.takeAt(index) :
                //Index is invalid, then give back a null pointer.
                nullptr;
}

void KMFlowLayout::clearItems()
{
    //Prepare the item.
    QLayoutItem *item;
    //Get all the item until the last item.
    while((item = takeAt(0)))
    {
        //Remove the item.
        delete item;
    }
}

Qt::Orientations KMFlowLayout::expandingDirections() const
{
    //There's no expanding direction for flow layout.
    return 0;
}

bool KMFlowLayout::hasHeightForWidth() const
{
    //It contains height for width.
    return true;
}

int KMFlowLayout::heightForWidth(int width) const
{
    //Calculate the height for width.
    return doLayout(QRect(0, 0, width, 0), true);
}

void KMFlowLayout::setGeometry(const QRect &rect)
{
    //Set the original geometry.
    QLayout::setGeometry(rect);
    //Change the layout.
    doLayout(rect, false);
}

QSize KMFlowLayout::sizeHint() const
{
    //The minimum size is the size hint.
    return minimumSize();
}

QSize KMFlowLayout::minimumSize() const
{
    //Prepare the empty size and item pointer.
    QSize size;
    QLayoutItem *item;
    //Add all the item together to be the flow layout size hint.
    foreach (item, itemList)
    {
        //Add size to size.
        size = size.expandedTo(item->minimumSize());
    }
    //Add margin to size.
    size += QSize(2*margin(), 2*margin());
    //Give back the size.
    return size;
}

inline int KMFlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    //Get the content margin of the current layout.
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    //Get the content rect size.
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x(),
        y = effectiveRect.y(),
        lineHeight = 0;
    //Initial the first line state.
    bool noMoreFirstLine=false;

    //Get all the item in the item list.
    QLayoutItem *item;
    for(int i=0; i<itemList.size(); ++i)
    {
        //Get the item.
        item=itemList.at(i);
        //Get the item to the widget.
        QWidget *currentWidget = item->widget();
        //Check whether folding is turn on.
        if(m_fold && noMoreFirstLine)
        {
            //Hide the current widget.
            currentWidget->hide();
            //Continue to next widget.
            continue;
        }
        //Initial the spacing.
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
        {
            spaceX = currentWidget->style()->layoutSpacing(
                        QSizePolicy::PushButton,
                        QSizePolicy::PushButton,
                        Qt::Horizontal);
        }
        int spaceY = verticalSpacing();
        if (spaceY == -1)
        {
            spaceY = currentWidget->style()->layoutSpacing(
                        QSizePolicy::PushButton,
                        QSizePolicy::PushButton,
                        Qt::Vertical);
        }
        //Calculate the prefer next X position.
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
        {
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        }

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

inline int KMFlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    //Get the parent object.
    QObject *parent = this->parent();
    if (!parent)
    {
        return -1;
    }
    else if (parent->isWidgetType())
    {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    }
    else
    {
        return static_cast<QLayout *>(parent)->spacing();
    }
}

bool KMFlowLayout::isFoldEnabled() const
{
    return m_fold;
}

void KMFlowLayout::setFold(bool fold)
{
    //Update the folded state.
    m_fold = fold;
    //Do the layout again.
//    doLayout();
}
