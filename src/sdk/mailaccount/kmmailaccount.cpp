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
#include "kmmaillistmodel.h"
#include "kmmailaccount.h"
#include "kmutil.h"
#include "kmglobal.h"

#include <QDebug>

KMMailAccount::KMMailAccount(QObject *parent) :
    QObject(parent),
    m_dirName(QString())
{
    //Build system folder models.
    for(int i=0; i<MailSystemFoldersCount; ++i)
    {
        //Construct the model.
        m_systemFolder[i]=new KMMailListModel(this);
    }
    //Set the system folder name.
    m_systemFolder[FolderInbox]->setFolderName("Inbox");
    m_systemFolder[FolderDrafts]->setFolderName("Drafts");
    m_systemFolder[FolderSentItems]->setFolderName("SentItems");
    m_systemFolder[FolderTrash]->setFolderName("Trash");
}

KMMailAccount::~KMMailAccount()
{
    //Get the account folder root dir.
    QString accountDir=kmGlobal->dirPath(KMGlobal::UserDataDir)+"/"+m_dirName;
    //Save the folder settings.
    for(int i=0; i<MailSystemFoldersCount; ++i)
    {
        //Save the system folder.
        m_systemFolder[i]->saveFolderData(accountDir);
    }
    //Save the customer folder data.
    for(int i=0; i<m_customFolder.size(); ++i)
    {
        //Save the folder info.
        m_customFolder.at(i)->saveFolderData(accountDir);
    }
    //Recover the memory from the list.
    clearCustomerFolder();
}

QString KMMailAccount::accountProperty(int propertyIndex) const
{
    Q_ASSERT(propertyIndex>-1 && propertyIndex<MailAccountPropertiesCount);
    //Get the property.
    return m_properties[propertyIndex];
}

KMMailListModel *KMMailAccount::systemFolder(int folderIndex)
{
    Q_ASSERT(folderIndex>-1 && folderIndex<MailSystemFoldersCount);
    //Get the folder model.
    return m_systemFolder[folderIndex];
}

KMMailListModel *KMMailAccount::customFolder(int customerIndex)
{
    Q_ASSERT(customerIndex>-1 && customerIndex<m_customFolder.size());
    //Get the customer folder model.
    return m_customFolder.at(customerIndex);
}

void KMMailAccount::setAccountProperty(int propertyIndex, const QString &value)
{
    Q_ASSERT(propertyIndex>-1 && propertyIndex<MailAccountPropertiesCount);
    //Save the property value.
    m_properties[propertyIndex]=value;
    //Emit the changed signal.
    emit propertyChanged();
}

void KMMailAccount::clearCustomerFolder()
{
    //Remove all customer folder model.
    qDeleteAll(m_customFolder);
    //Clear the folder list.
    m_customFolder.clear();
}

void KMMailAccount::setDirName(const QString &folderName)
{
    //Save folder name.
    m_dirName = KMUtil::validFileName(folderName);
}

QString KMMailAccount::dirName() const
{
    return m_dirName;
}
