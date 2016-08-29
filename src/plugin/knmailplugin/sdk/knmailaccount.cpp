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
#include <QDir>

#include "knutil.h"

#include "knmailmodel.h"
#include "knmailglobal.h"

#include "knmailaccount.h"

#include <QDebug>

#define AccountFolder (knMailGlobal->mailAccountFolder() + "/" + m_username)

KNMailAccount::KNMailAccount(QObject *parent) :
    QObject(parent),
    m_username(QString()),
    m_password(QString()),
    m_displayName(QString()),
    m_provider(QString())
{
    //Construct all the mail models.
    for(int i=0; i<DefaultFolderCount; ++i)
    {
        //Construct the item.
        m_defaultFolders[i]=new KNMailModel(this);
        //Set the folder index.
        m_defaultFolders[i]->setDefaultFolderIndex(i);
        m_defaultFolders[i]->setManagedAccount(this);
    }
}

KNMailProtocolConfig KNMailAccount::sendConfig() const
{
    return m_sendConfig;
}

void KNMailAccount::setSendConfig(const KNMailProtocolConfig &sendConfig)
{
    m_sendConfig = sendConfig;
}

KNMailProtocolConfig KNMailAccount::receiveConfig() const
{
    return m_receiveConfig;
}

void KNMailAccount::setReceiveConfig(const KNMailProtocolConfig &receiveConfig)
{
    m_receiveConfig = receiveConfig;
}

void KNMailAccount::setCustomFolders(const QList<KNMailModel *> &customFolders)
{
    //Record the previous size.
    int previousSize=m_customFolders.size();
    //Update the folder list.
    m_customFolders = customFolders;
    //Check the list size.
    if(previousSize!=m_customFolders.size())
    {
        //Emit the folder count changed signal.
        emit folderCountChanged();
    }
}

void KNMailAccount::saveFolder()
{
    //Get the account folder.
    QString accountFolder=KNUtil::ensurePathValid(AccountFolder);
    //Check the accout folder.
    if(accountFolder.isEmpty())
    {
        //Failed to find the folder.
        return;
    }
    //Save the folder content.
    for(int i=0; i<DefaultFolderCount; ++i)
    {
        //Save the file content.
        m_defaultFolders[i]->saveToFolder(accountFolder);
    }
    //Save the custom folder.
    for(auto i : m_customFolders)
    {
        //Save the custom folder.
        i->saveToFolder(accountFolder);
    }
}

QList<KNMailModel *> KNMailAccount::customFolders() const
{
    return m_customFolders;
}

QString KNMailAccount::receiveProtocolName() const
{
    return m_receiveProtocolName;
}

void KNMailAccount::setReceiveProtocolName(const QString &receiveProtocolName)
{
    m_receiveProtocolName = receiveProtocolName;
}

QString KNMailAccount::sendProtocolName() const
{
    return m_sendProtocolName;
}

void KNMailAccount::setSendProtocolName(const QString &sendProtocolName)
{
    m_sendProtocolName = sendProtocolName;
}

QString KNMailAccount::provider() const
{
    return m_provider;
}

KNMailModel *KNMailAccount::folder(int index)
{
    //Check the index.
    return index<DefaultFolderCount?
                m_defaultFolders[index]:
                m_customFolders.at(index-DefaultFolderCount);
}

void KNMailAccount::setProvider(const QString &provider)
{
    m_provider = provider;
}

QString KNMailAccount::password() const
{
    return m_password;
}

QString KNMailAccount::displayString() const
{
    return m_displayName + " <" + m_username + ">";
}

QString KNMailAccount::accountDirectoryPath() const
{
    return AccountFolder;
}

int KNMailAccount::folderCount()
{
    return DefaultFolderCount + m_customFolders.size();
}

QString KNMailAccount::folderName(int folderIndex)
{
    return folderIndex<DefaultFolderCount?
                m_defaultFolders[folderIndex]->folderName() :
                m_customFolders.at(
                    folderIndex-DefaultFolderCount)->folderName();
}

void KNMailAccount::setPassword(const QString &password)
{
    m_password = password;
}

QString KNMailAccount::username() const
{
    return m_username;
}

void KNMailAccount::setUsername(const QString &username)
{
    //Save the user name.
    m_username = username;
    //Get the account folder.
    QString accountFolder=AccountFolder;
    //Check the accout folder.
    if(accountFolder.isEmpty())
    {
        //Failed to find the folder.
        return;
    }
    //Update the directory information according to the username, get the file
    //info list.
    QFileInfoList folderList=QDir(accountFolder).entryInfoList();
    //List the directory from the folder.
    for(auto i:folderList)
    {
        //Ignore . and ..
        if(i.fileName()=="." || i.fileName()=="..")
        {
            continue;
        }
        qDebug()<<i.fileName();
        //Check the content type.
        if(i.isDir())
        {
            //Check the file name.
            if(i.fileName()=="Inbox")
            {
                //Load the inbox content.
                m_defaultFolders[FolderInbox]->loadFromFolder(accountFolder);
            }
            else if(i.fileName()=="SentItems")
            {
                //Load the sent items content.
                m_defaultFolders[FolderSentItems]->loadFromFolder(
                            accountFolder);
            }
            else if(i.fileName()=="Drafts")
            {
                //Load the draft content.
                m_defaultFolders[FolderDrafts]->loadFromFolder(accountFolder);
            }
            else if(i.fileName()=="Trash")
            {
                //Load the trash content.
                m_defaultFolders[FolderTrash]->loadFromFolder(accountFolder);
            }
            else if(i.fileName()=="Junk")
            {
                //Load the junk content.
                m_defaultFolders[FolderJunk]->loadFromFolder(accountFolder);
            }
            else
            {
                //Contruct a new model.
                KNMailModel *folderModel=new KNMailModel(this);
                //Set the folder model name.
                folderModel->setFolderName(i.fileName());
                folderModel->setManagedAccount(this);
                //Load the folder content.
                folderModel->loadFromFolder(accountFolder);
                //Add model to account.
                m_customFolders.append(folderModel);
            }
        }
    }
    //Emit the update signal.
    emit folderCountChanged();
}

QString KNMailAccount::displayName() const
{
    return m_displayName;
}

void KNMailAccount::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}
