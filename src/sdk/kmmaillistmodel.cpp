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
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>

#include "kmutil.h"

#include "kmmaillistmodel.h"

KMMailListModel::KMMailListModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

KMMailListModel::~KMMailListModel()
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
    case SenderRole:
        return m_mailLists.at(index.row()).sender;
    case BreifContextRole:
        return m_mailLists.at(index.row()).breifContext;
    default:
        return QVariant();
    }
}

void KMMailListModel::appendRow(const MailListItem &item)
{
    //Start append the item.
    beginInsertRows(QModelIndex(), m_mailLists.size(), m_mailLists.size());
    //Append the item.
    m_mailLists.append(item);
    //End append the item.
    endInsertRows();
}

void KMMailListModel::setFolderName(const QString &folderName)
{
    //Check the previous folder name.
    if(!m_folderName.isEmpty())
    {
        //! FIXME: Add rename codes here.
    }
    //Save the folder name.
    m_folderName=folderName;
}

void KMMailListModel::saveFolderData(const QString &folderPath)
{
    //Combine the folder path together.
    QString targetPath=folderPath + "/" + m_folderName;
    //Get target folder information.
    QFileInfo targetInfo(targetPath);
    //Check whether target path is existed or not.
    if(!targetInfo.exists())
    {
        //Create target folder.
        if(KMUtil::ensurePathValid(targetInfo.absoluteFilePath()).isEmpty())
        {
            //Failed to create folder.
            return;
        }
    }
    //Generate a QDir object.
    QDir targetDir(targetInfo.absoluteFilePath());
    //Get the configure file.
    QFile infoFile(targetDir.absoluteFilePath("info.json"));
    //Open the info file as write only mode.
    if(!infoFile.open(QIODevice::WriteOnly))
    {
        //Failed to write configure.
        return;
    }
    //Generate the info JSON object.
    QJsonObject folderInfo;
    //Set information to JSON object.
    folderInfo.insert("Name", m_folderName);
    //Save the data to info file.
    infoFile.write(QJsonDocument(folderInfo).toJson(QJsonDocument::Compact));
    //Close the file.
    infoFile.close();
}
