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
#include "knmailmodel.h"

#include "knmailaccount.h"

KNMailAccount::KNMailAccount(QObject *parent) :
    QObject(parent)
{
    //Construct all the mail models.
    for(int i=0; i<DefaultFolderCount; ++i)
    {
        //Construct the item.
        m_defaultFolders[i]=new KNMailModel(this);
        //Set the folder index.
        m_defaultFolders[i]->setDefaultFolderIndex(i);
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

QString KNMailAccount::provider() const
{
    return m_provider;
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
    m_username = username;
}

QString KNMailAccount::displayName() const
{
    return m_displayName;
}

void KNMailAccount::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}