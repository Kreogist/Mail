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

#include "mime/kmmailparseutil.h"
#include "mime/kmmimemailparser.h"
#include "mime/kmmimepart.h"
#include "mime/kmmimemultipart.h"
#include "mailaccount/kmmailaccount.h"
#include "kmglobal.h"
#include "kmutil.h"

#include "kmmaillistmodel.h"

#include <QDebug>

KMMailListModel::KMMailListModel(KMMailAccount *parent) :
    QAbstractListModel(parent),
    m_folderName(QString()),
    m_dirName(QString()),
    m_loaded(false)
{
    //Save the account information.
    m_account=parent;
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
        return m_mailLists.at(index.row()).briefContext;
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

void KMMailListModel::setDirectory(const QString &dirName)
{
    //Check the previous folder name.
    if(!m_dirName.isEmpty())
    {
        //! FIXME: Add rename codes here.
    }
    //Save the folder name.
    m_dirName=dirName;
}

void KMMailListModel::saveFolderData()
{
    //Combine the folder path together.
    QString targetPath=m_account->accountDir() + "/" + m_dirName;
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

bool KMMailListModel::loaded() const
{
    return m_loaded;
}

void KMMailListModel::initial()
{
    //Check load flag.
    if(m_loaded)
    {
        //Ignore the loaded flag.
        return;
    }
    //Clear the model.
    clear();
    //Load all the mails to the model.
    QDir modelDirectory(m_account->accountDir() + "/" + m_dirName);
    //Check directory exist.
    if(!modelDirectory.exists())
    {
        //When the directory is not exist, exit.
        return;
    }
    //Get the file info list.
    QFileInfoList mailInfoList=
            modelDirectory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    //Check the mail info list.
    for(auto i=mailInfoList.begin(); i!=mailInfoList.end(); ++i)
    {
        //Get the info list size.
        if((*i).suffix()!="eml" && (*i).suffix()!="emlx")
        {
            //Ignore the file.
            continue;
        }
        //Check whether file is already load to model.
        //!FIXME: Replace to check cache code here.
        if(true)
        {
            //Add the new mail informationn to the inbox model.
            MailListItem mailItem;
            mailItem.fileName=(*i).fileName();
            mailItem.index=(*i).fileName().toLongLong();
            //Parse the mail data.
            KMMimePart *parseResult=
                    KMMimeMailParser::parseContent((*i).absoluteFilePath(),
                                                   nullptr);
            //Check parse result.
            if(parseResult)
            {
                //Get one property.
                mailItem.title=KMMailParseUtil::parseEncoding(
                            parseResult->mimeProperty("subject"));
                mailItem.sender=KMMailParseUtil::parseEncoding(
                            parseResult->mimeProperty("from"));
                //Check the from data.
                int addressStart=mailItem.sender.indexOf('<');
                //If the address start doesn't contains the start char, it
                //should be a bug.
                if(addressStart!=-1)
                {
                    //Rebuild the item.
                    mailItem.sender=
                            mailItem.sender.left(addressStart).simplified()
                            + " " + mailItem.sender.mid(addressStart);
                }
                //Get the brief content.
                KMMimeMailParser::getBriefContent(parseResult,
                                                  mailItem.briefContext);
                //Remove the parse result;
                delete parseResult;
                //Add item to model.
                appendRow(mailItem);
            }
        }
    }
}

void KMMailListModel::clear()
{
    //Check is already clean or not.
    if(m_mailLists.isEmpty())
    {
        //No need to clear.
        return;
    }
    //Start to reset model.
    beginResetModel();
    //Clear the lists.
    m_mailLists.clear();
    //Mission complete.
    endResetModel();
}

QString KMMailListModel::folderName() const
{
    return m_folderName;
}

void KMMailListModel::setFolderName(const QString &folderName)
{
    //Save the folder name.
    m_folderName = folderName;
    //Emit the signal.
    emit folderNameChanged(m_folderName);
}
