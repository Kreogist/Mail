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
#include "knlocalemanager.h"
#include "kmmaillistmodel.h"
#include "kmmailaccount.h"
#include "kmutil.h"
#include "kmglobal.h"

#include <QDebug>

KMMailAccount::KMMailAccount(QObject *parent) :
    QObject(parent),
    m_dirName(QString())
{
    //Debug.
    QString sampleSender[MailSystemFoldersCount];
    sampleSender[FolderInbox]="Mayazure Jerry <jerry@anu.edu.au>";
    sampleSender[FolderDrafts]="Tojo Saki <saki.tojo@anu.edu.au>";
    sampleSender[FolderTrash]="Freddie Wang <freddie@anu.edu.au>";
    sampleSender[FolderSentItems]="Shayne Flint <shayne@anu.edu.au>";

    //Build system folder models.
    for(int i=0; i<MailSystemFoldersCount; ++i)
    {
        //Construct the model.
        m_systemFolder[i]=new KMMailListModel(this);

        //Add some data for debug.
        MailListItem sampleMail;
        sampleMail.title="Sample Mail " + QString::number(i) + QString::number(qrand());
        sampleMail.sender=sampleSender[i];
        sampleMail.breifContext="This is a sample mail item for test only, it is not a real mail.";
        for(int j=0; j<40; ++j)
        {
            m_systemFolder[i]->appendRow(sampleMail);
        }
    }
    //Set the system directory.
    m_systemFolder[FolderInbox]->setDirectory("Inbox");
    m_systemFolder[FolderDrafts]->setDirectory("Drafts");
    m_systemFolder[FolderSentItems]->setDirectory("SentItems");
    m_systemFolder[FolderTrash]->setDirectory("Trash");
    //Link retranslator.
    knI18n->link(this, &KMMailAccount::retranslate);
    retranslate();
}

KMMailAccount::~KMMailAccount()
{
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

int KMMailAccount::customerFolderSize() const
{
    return m_customFolder.size();
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
    for(auto i=m_customFolder.begin(); i!=m_customFolder.end(); ++i)
    {
        //Delete the model.
        (*i)->deleteLater();
    }
    //Clear the folder list.
    m_customFolder.clear();
}

void KMMailAccount::saveAccountData()
{
    //Save the folder settings.
    for(int i=0; i<MailSystemFoldersCount; ++i)
    {
        //Save the system folder.
        m_systemFolder[i]->saveFolderData();
    }
    //Save the customer folder data.
    for(int i=0; i<m_customFolder.size(); ++i)
    {
        //Save the folder info.
        m_customFolder.at(i)->saveFolderData();
    }
}

void KMMailAccount::retranslate()
{
    //Update the system folder name.
    m_systemFolder[FolderInbox]->setFolderName(tr("Inbox"));
    m_systemFolder[FolderDrafts]->setFolderName(tr("Drafts"));
    m_systemFolder[FolderSentItems]->setFolderName(tr("Sent Items"));
    m_systemFolder[FolderTrash]->setFolderName(tr("Trash"));
}

QString KMMailAccount::accountDir() const
{
    return m_accountDir;
}

void KMMailAccount::setDirName(const QString &folderName)
{
    //Save folder name.
    m_dirName = KMUtil::validFileName(folderName);
    //Get the account folder root dir.
    m_accountDir=kmGlobal->dirPath(KMGlobal::UserDataDir)+"/Accounts/"+
            m_dirName;
}

QString KMMailAccount::dirName() const
{
    return m_dirName;
}
