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
#include "knmailglobal.h"

#include "knmailmodel.h"

KNMailModel::KNMailModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_folderPath(QString()),
    m_folderName(QString()),
    m_defaultFolderIndex(-1)
{
}

QString KNMailModel::folderPath() const
{
    return m_folderPath;
}

int KNMailModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Directly give the list size back.
    return m_itemList.size();
}

int KNMailModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Directly give the column size back.
    return FolderModelColumnCount;
}

QVariant KNMailModel::data(const QModelIndex &index, int role) const
{
    //Check the index first.
    if(!index.isValid())
    {
        //For invalid index, give back invalid data directly.
        return QVariant();
    }
    //Get the target item.
    const KNMailListItem &item=m_itemList.at(index.row());
    //Check the role.
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        //Check the column:
        switch(index.column())
        {
        case ColumnTitle:
            return item.title;
        case ColumnFlag:
            return false;
        case ColumnSender:
            return item.sender;
        case ColumnReceiveDate:
            return QString();
        }

    default:
        return QVariant();
    }
}

void KNMailModel::appendRow(const KNMailListItem &item)
{
    //Follow the documentation, we have to do this.
    //Remember the index here should be the same.
    //Url link:
    //https://forum.qt.io/topic/28850/qtreeview-with-qsortfilterproxymodel-displ
    //ays-an-empty-row-after-changing-the-qabstractitemmodel/5
    //Copy of the link:
    /*
     * The issue here is that you tell your model that you are inserting two
     *  rows:
     * @this->beginInsertRows(QModelIndex() ,
     *                        rootItem->childCount(),
     *                        rootItem->childCount() + 1);@
     * According to
     * http://qt-project.org/doc/qt-4.8/qabstractitemmodel.html#beginInsertRows,
     * you have to specify the first and last row number. Which, if you add only
     * one row, should be the same. Remove the +1, it will probably solve your
     * issue.
     */
    beginInsertRows(QModelIndex(), m_itemList.size(), m_itemList.size());
    //Append the data at the end of the list.
    m_itemList.append(item);
    //As the documentation said, called this after insert rows.
    endInsertRows();
}

void KNMailModel::appendRows(const QList<KNMailListItem> &items)
{
    //Ignore the empty adding request.
    if(items.isEmpty())
    {
        return;
    }
    //Follow the documentation, we have to do this at first.
    beginInsertRows(QModelIndex(),
                    m_itemList.size(),
                    m_itemList.size() + items.size() - 1);
    //Add all the data at the end of the list.
    m_itemList.append(items);
    //Call this after insert rows.
    endInsertRows();
}

void KNMailModel::setFolderPath(const QString &folderPath)
{
    m_folderPath = folderPath;
}

int KNMailModel::defaultFolderIndex() const
{
    return m_defaultFolderIndex;
}

void KNMailModel::setDefaultFolderIndex(int defaultFolderIndex)
{
    m_defaultFolderIndex = defaultFolderIndex;
}

QString KNMailModel::folderName() const
{
    return m_defaultFolderIndex==-1?
                m_folderName:
                knMailGlobal->defaultFolderName(m_defaultFolderIndex);
}

void KNMailModel::setFolderName(const QString &folderName)
{
    m_folderName = folderName;
}
