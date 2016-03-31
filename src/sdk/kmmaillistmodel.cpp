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
#include "kmmaillistmodel.h"

KMMailListModel::KMMailListModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int KMMailListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Give back the list size.
    return m_mailLists.size();
}

QVariant KMMailListModel::data(const QModelIndex &index,
                               int role) const
{
    //Check out the index is valid or not.
    if(!index.isValid())
    {
        //Give a invalid data back.
        return QVariant();
    }
    //Check the role.
    switch(role)
    {
    //Display role = Mail title.
    case Qt::DisplayRole:
        return m_mailLists.at(index.row()).title;
    default:
        return QVariant();
    }
}

void KMMailListModel::appendRow(const KMMailUtil::MailListItem &item)
{
    //Start append the item.
    beginInsertRows(QModelIndex(), m_mailLists.size(), m_mailLists.size());
    //Append the item.
    m_mailLists.append(item);
    //End append the item.
    endInsertRows();
}
