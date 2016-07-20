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

#include "knmailaccount.h"
#include "knmailaccountmanager.h"

#include "knmailglobal.h"

KNMailGlobal *KNMailGlobal::m_instance=nullptr;

KNMailGlobal *KNMailGlobal::instance()
{
    return m_instance;
}

void KNMailGlobal::initial(QObject *parent)
{
    //Check if the singleton instance variable is null. Set the pointer to this
    //object if this is the first constructed object.
    if(m_instance==nullptr)
    {
        m_instance=new KNMailGlobal(parent);
    }
}

QString KNMailGlobal::defaultFolderName(int index)
{
    Q_ASSERT(index>-1 && index<DefaultFolderCount);
    //Give back the folder name.
    return m_defaultFolderName[index];
}

QString KNMailGlobal::viewerTitleText(int index)
{
    Q_ASSERT(index>-1 && index<MailViewerTitleFieldCount);
    //Give back the title field name.
    return m_titleFieldText[index];
}

QPixmap KNMailGlobal::providerIcon(const QString &providerName)
{
    return m_providerIcon.value(providerName);
}

void KNMailGlobal::retranslate()
{
    //Update the default folder name.
    m_defaultFolderName[FolderInbox]=tr("Inbox");
    m_defaultFolderName[FolderDrafts]=tr("Drafts");
    m_defaultFolderName[FolderSentItems]=tr("Sent Items");
    m_defaultFolderName[FolderTrash]=tr("Trash");
    //Update the title field text.
    m_titleFieldText[FieldFrom]=tr("From");
    m_titleFieldText[FieldReceive]=tr("To");
    m_titleFieldText[FieldCarbonCopy]=tr("CC");
}

KNMailGlobal::KNMailGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial the infrastructures.
    KNMailAccountManager::initial(this);

    //Initial the provider icons.
    m_providerIcon.insert("netease",
                          QPixmap(":/plugin/mail/providers/netease.png"));
    m_providerIcon.insert("google",
                          QPixmap(":/plugin/mail/providers/gmail.png"));

    //Link retranslate.
    knI18n->link(this, &KNMailGlobal::retranslate);
    retranslate();
}
