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
#include <QPainter>

#include "kmmailutil.h"

#include "kmmaillistviewdelegate.h"

#include <QDebug>

using namespace MailUtil;

#define Spacing 4
#define TopMargin 7
#define LeftMargin 19

KMMailListViewDelegate::KMMailListViewDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void KMMailListViewDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    //Draw the title with a larger font.
    QFont titleFont=option.font;
    //Save the title font height.
    int contentFontHeight=option.fontMetrics.height(),
        titleFontHeight=(qreal)contentFontHeight/0.75,
        widgetWidth=option.widget->width()-(LeftMargin<<1);
    titleFont.setPixelSize(titleFontHeight);
    //Check if the current row is the selected row, then draw the selection
    //bound.
    if(option.state & QStyle::State_Selected)
    {
        //Draw the selected background.
        painter->fillRect(QRect(option.rect.x(),
                                option.rect.y(),
                                option.widget->width(),
                                option.rect.height()),
                          option.palette.color(QPalette::Highlight));
    }
    else
    {
        //Check if this row is odd or oven, and then take one row to have
        //alternative background.
        if(index.row() & 1)
        {
            //Draw the alternative background.
            painter->fillRect(QRect(option.rect.x(),
                                    option.rect.y(),
                                    option.widget->width(),
                                    option.rect.height()),
                              option.palette.color(QPalette::AlternateBase));
        }
    }
    painter->save();
    //Draw the receiver.
    painter->drawText(option.rect.left()+LeftMargin,
                      option.rect.top()+titleFontHeight+Spacing+
                      contentFontHeight+TopMargin,
                      option.fontMetrics.elidedText(
                          index.data(SenderRole).toString(),
                          Qt::ElideRight,
                          widgetWidth));
    //Draw the brief text.
    painter->drawText(option.rect.left()+LeftMargin,
                      option.rect.top()+titleFontHeight+(Spacing<<1)+
                      (contentFontHeight<<1)+TopMargin,
                      option.fontMetrics.elidedText(
                          index.data(BreifContextRole).toString(),
                          Qt::ElideRight,
                          widgetWidth));
    //Draw the title font.
    painter->setFont(titleFont);
    painter->drawText(option.rect.left()+LeftMargin,
                      option.rect.top()+titleFontHeight+TopMargin,
                      QFontMetrics(titleFont).elidedText(
                                        index.data(Qt::DisplayRole).toString(),
                                        Qt::ElideRight,
                                        widgetWidth));
    //Draw the bottom line.
    painter->setPen(QColor(0, 0, 0));
    painter->drawLine(option.rect.bottomLeft(),
                      option.rect.bottomRight());
    painter->restore();
}

QSize KMMailListViewDelegate::sizeHint(const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    Q_UNUSED(index)
    //Calculate the size hint.
    return QSize(0,
                 option.fontMetrics.height()*4 + (Spacing*3)+TopMargin);
}
