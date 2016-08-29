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
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

#include "knmailglobal.h"
#include "knutil.h"

#include "knmailmodel.h"

#include <QDebug>

#define ItemSender "S"
#define ItemSenderName "SN"
#define ItemReceiver "R"
#define ItemReceiverName "RN"
#define ItemTitle "T"
#define ItemBriefContent "B"
#define ItemIndex "I"
#define ItemLoaded "L"

KNMailModel::KNMailModel(QObject *parent) :
    QAbstractTableModel(parent),
    m_folderName(QString()),
    m_defaultFolderIndex(-1)
{
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
            return item.senderName.isEmpty()?
                        item.sender:
                        item.senderName;
        case ColumnReceiveDate:
            return QString();
        }
    case MailPathRole:
        return QString::number(item.uid);
    default:
        return QVariant();
    }
}

QVariant KNMailModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    //Check the orientation.
    if(orientation==Qt::Vertical)
    {
        //No useful vertical header data.
        return QVariant();
    }
    //Check the role and section.
    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        //Check the column:
        switch(section)
        {
        case ColumnTitle:
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
                knMailGlobal->defaultFolderDisplayName(m_defaultFolderIndex);
}

void KNMailModel::setFolderName(const QString &folderName)
{
    m_folderName = folderName;
}

void KNMailModel::loadFromFolder(const QString &accountFolder)
{
    //Load the configuration from the folder.
    QFile metaDataFile(
                accountFolder + "/" +
                ((m_defaultFolderIndex==-1)?
                     m_folderName:
                     knMailGlobal->defaultFolderName(m_defaultFolderIndex))
                + "/info.json");
    //Load the content from file.
    if(!metaDataFile.open(QIODevice::ReadOnly))
    {
        //It cannot open the file as read only mode.
        return;
    }
    //Load the data, translate the to list item.
    m_itemArray=QJsonDocument::fromJson(metaDataFile.readAll()).array();
    //Close the file.
    metaDataFile.close();
    //Reset the content.
    reset();
    //Check the item array size.
    if(m_itemArray.isEmpty())
    {
        //Ignore the item array.
        return;
    }
    //Start to insert content.
    beginInsertRows(QModelIndex(), 0, m_itemArray.size()-1);
    //Check all the array.
    for(auto i:m_itemArray)
    {
        //Translate the item
        QJsonObject itemData=i.toObject();
        //Generate the item.
        KNMailListItem item;
        //Construct the item.
        item.sender=itemData.value(ItemSender).toString();
        item.senderName=itemData.value(ItemSenderName).toString();
        item.receiver=itemData.value(ItemReceiver).toString();
        item.receiverName=itemData.value(ItemReceiverName).toString();
        item.title=itemData.value(ItemTitle).toString();
        item.breifContext=itemData.value(ItemBriefContent).toString();
        item.uid=itemData.value(ItemIndex).toInt();
        item.cached=itemData.value(ItemLoaded).toBool();
        //Add the item to list.
        m_itemList.append(item);
    }
    //Finish insert the content.
    endInsertRows();
    //Recover the memory.
    m_itemArray=QJsonArray();
}

void KNMailModel::saveToFolder(const QString &accountFolder)
{
    //Get the folder path.
    QString folderPath=KNUtil::ensurePathValid(
                accountFolder + "/" +
                ((m_defaultFolderIndex==-1)?
                     m_folderName:
                     knMailGlobal->defaultFolderName(m_defaultFolderIndex)));
    //Check the folder path.
    if(folderPath.isEmpty())
    {
        //Failed to get the folder path.
        return;
    }
    //Construct the item array.
    for(auto item : m_itemList)
    {
        //Translate the item
        QJsonObject itemData;
        //Construct the item.
        itemData.insert(ItemSender, item.sender);
        itemData.insert(ItemSenderName, item.senderName);
        itemData.insert(ItemReceiver, item.receiver);
        itemData.insert(ItemReceiverName, item.receiverName);
        itemData.insert(ItemTitle, item.title);
        itemData.insert(ItemBriefContent, item.breifContext);
        itemData.insert(ItemIndex, item.uid);
        itemData.insert(ItemLoaded, item.cached);
        //Add the item to list.
        m_itemArray.append(itemData);
    }
    //Load the configuration from the folder.
    QFile metaDataFile(folderPath + "/info.json");
    //Open for write.
    if(metaDataFile.open(QIODevice::WriteOnly))
    {
        //Write the content to the meta data file.
        metaDataFile.write(QJsonDocument(m_itemArray).toJson());
    }
    //Close the file.
    metaDataFile.close();
    //Clear the array.
    m_itemArray=QJsonArray();
}

void KNMailModel::reset()
{
    //Check the list.
    if(m_itemList.isEmpty())
    {
        //Ignore the list.
        return;
    }
    //Start to reset the content.
    beginResetModel();
    //Clear the content.
    m_itemList.clear();
    //End the reset content.
    endResetModel();
}

void KNMailModel::removeModelContent(const QString &accountFolder)
{
    //Remove the folder.
    QDir(accountFolder + "/" +
         ((m_defaultFolderIndex==-1)?
              m_folderName:
              knMailGlobal->defaultFolderName(m_defaultFolderIndex))).
            removeRecursively();
}

void KNMailModel::updateUidList(const QString &accountFolder,
                                QList<int> *uidList)
{
    //Generate the item list.
    QList<KNMailListItem> *updateList=new QList<KNMailListItem>();
    //Get the folder path.
    QString folderPath=accountFolder + "/" +
            ((m_defaultFolderIndex==-1)?
                 m_folderName:
                 knMailGlobal->defaultFolderName(m_defaultFolderIndex));
    //Check until all the content has been removed.
    while(!uidList->isEmpty() && !m_itemList.isEmpty())
    {
        //Get the last content.
        int currentUid=uidList->takeLast();
        //Check the uid.
        while(!m_itemList.isEmpty() && m_itemList.last().uid<currentUid)
        {
            //Remove the last index.
            KNMailListItem lastItem=m_itemList.takeLast();
            //Delete the mail file if the file is cached.
            if(lastItem.cached)
            {
                //Get the cache folder name and file name.
                QString mailName=QString::number(lastItem.uid);
                //Delete the file.
                QFile::remove(folderPath + "/" + mailName + ".eml");
                //Delete the cache folder.
                QDir(folderPath + "/" + mailName).removeRecursively();
            }
        }
        //Check if the item list is empty or not.
        if(!m_itemList.isEmpty())
        {
            //When it is not empty, the last one must has the same uid as the
            //current uid.
            updateList->prepend(m_itemList.takeLast());
        }
    }
    //When all the content is complete, all the id left in the uid list will be
    //the content left in the new item.
    //And these mail will be the new mail need to update.
    while(!uidList->isEmpty())
    {
        //Generate the new item for them.
        KNMailListItem currentItem;
        //Set the uid.
        currentItem.uid=uidList->takeLast();
        currentItem.title=QString::number(currentItem.uid);
        //Prepend to the current list.
        updateList->prepend(currentItem);
    }
    //Update the item list.
    m_itemList=(*updateList);
    //Remove the update list.
    delete updateList;
}

QString KNMailModel::serverName() const
{
    return m_serverName;
}

void KNMailModel::setServerName(const QString &serverName)
{
    m_serverName = serverName;
}
