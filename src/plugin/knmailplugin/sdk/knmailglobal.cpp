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
#include "knglobal.h"

#include "knmailprotocolmanager.h"
#include "knmailcomposegeneratorbase.h"
#include "knmailaccount.h"
#include "knmailpopupmanager.h"
#include "knmailaccountmanager.h"
#include "knmailviewergeneratorbase.h"
#include "knmailviewerbase.h"
#include "knmailwebviewergeneratorbase.h"
#include "knmailcomposermanager.h"
#include "knmailreceivermanager.h"
#include "knmailmodelupdater.h"

#include "knmailglobal.h"

#include <QDebug>

KNMailGlobal *KNMailGlobal::m_instance=nullptr;

KNMailGlobal::~KNMailGlobal()
{
    //Check the mail viewer generator pointer.
    if(m_viewerGenerator)
    {
        //Recover the memory.
        delete m_viewerGenerator;
        //Reset the pointer.
        m_viewerGenerator=nullptr;
    }
    //Check the web content generator pointer.
    if(m_webViewerGenerator)
    {
        //Recover the memory.
        delete m_webViewerGenerator;
        //Reset the pointer.
        m_webViewerGenerator=nullptr;
    }
    //Check the composer generator pointer.
    if(m_viewerGenerator)
    {
        //Recover the memory.
        delete m_viewerGenerator;
        //Reset the pointer.
        m_viewerGenerator=nullptr;
    }
    //Quit and wait the receiver thread.
    m_receiverThread.quit();
    m_receiverThread.wait();
    m_updaterThread.quit();
    m_updaterThread.wait();
    //Remove the receiver.
    knMailReceiverManager->deleteLater();
    //Recover the protocol manager object.
    knMailProtocolManager->deleteLater();
    //Recover the mail folder updater.
    knMailModelUpdater->deleteLater();
}

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

QString KNMailGlobal::defaultFolderDisplayName(int index) const
{
    Q_ASSERT(index>-1 && index<DefaultFolderCount);
    //Give back the folder name.
    return m_defaultFolderName[index];
}

QString KNMailGlobal::defaultFolderName(int index) const
{
    Q_ASSERT(index>-1 && index<DefaultFolderCount);
    //Give back the folder name.
    switch(index)
    {
    case FolderInbox:
        return "Inbox";
    case FolderDrafts:
        return "Drafts";
    case FolderSentItems:
        return "Sent";
    case FolderJunk:
        return "Junk";
    case FolderTrash:
        return "Trash";
    }
    return QString();
}

QString KNMailGlobal::mailAccountFolder() const
{
    return knGlobal->dirPath(KNGlobal::UserDataDir) + "/Accounts";
}

QString KNMailGlobal::viewerTitleText(int index)
{
    Q_ASSERT(index>-1 && index<MailViewerTitleFieldCount);
    //Give back the title field name.
    return m_titleFieldText[index];
}

QPixmap KNMailGlobal::providerIcon(const QString &providerName)
{
    return m_providerIcon.value(providerName, m_emtpyIcon);
}

KNMailViewerBase *KNMailGlobal::generateViewer()
{
    //Generate the viewer.
    KNMailViewerBase *mailViewer=m_viewerGenerator->generateViewer();
    //Add an web viewer.
    mailViewer->setWebViewer(generateWebViewer(mailViewer));
    //Return the viewer.
    return mailViewer;
}

KNMailViewerBase *KNMailGlobal::generatePopupViewer()
{
    //Generate the popup viewer.
    KNMailViewerBase *mailViewer=m_viewerGenerator->generatePopupViewer();
    //Add an web viewer.
    mailViewer->setWebViewer(generateWebViewer(mailViewer));
    //Return the viewer.
    return mailViewer;
}

void KNMailGlobal::retranslate()
{
    //Update the default folder name.
    m_defaultFolderName[FolderInbox]=tr("Inbox");
    m_defaultFolderName[FolderDrafts]=tr("Drafts");
    m_defaultFolderName[FolderSentItems]=tr("Sent");
    m_defaultFolderName[FolderJunk]=tr("Junk");
    m_defaultFolderName[FolderTrash]=tr("Trash");
    //Update the title field text.
    m_titleFieldText[FieldFrom]=tr("From");
    m_titleFieldText[FieldReceive]=tr("To");
    m_titleFieldText[FieldCarbonCopy]=tr("CC");
    //Update the subject.
    m_noSubject=tr("No subject");
}

KNMailGlobal::KNMailGlobal(QObject *parent) :
    QObject(parent),
    m_emtpyIcon(QPixmap(":/plugin/mail/providers/generic.png")),
    m_viewerGenerator(nullptr),
    m_webViewerGenerator(nullptr),
    m_composerGenerator(nullptr)
{
    //Register cached types.
    qRegisterMetaType<QVector<int>>("QVector<int>");

    //Initial the infrastructures.
    KNMailAccountManager::initial(this);
    KNMailPopupManager::initial(this);
    KNMailComposerManager::initial(this);
    KNMailModelUpdater::initial(&m_updaterThread);
    KNMailReceiverManager::initial(&m_receiverThread);
    KNMailProtocolManager::initial();

    //Initial the provider icons.
    m_providerIcon.insert("netease",
                          QPixmap(":/plugin/mail/providers/netease.png"));
    m_providerIcon.insert("google",
                          QPixmap(":/plugin/mail/providers/gmail.png"));

    //Link the update request.
    connect(this, &KNMailGlobal::requireUpdateAll,
            knMailReceiverManager, &KNMailReceiverManager::updateAllAccount);

    //Link retranslate.
    knI18n->link(this, &KNMailGlobal::retranslate);
    retranslate();
}

QString KNMailGlobal::noSubjectText() const
{
    return m_noSubject;
}

void KNMailGlobal::setComposerGenerator(
        KNMailComposeGeneratorBase *composerGenerator)
{
    m_composerGenerator = composerGenerator;
}

void KNMailGlobal::setWebViewerGenerator(
        KNMailWebViewerGeneratorBase *webViewerGenerator)
{
    m_webViewerGenerator = webViewerGenerator;
}

KNMailComposeBase *KNMailGlobal::generateComposer()
{
    return m_composerGenerator->generateComposer();
}

void KNMailGlobal::setViewerGenerator(
        KNMailViewerGeneratorBase *viewerGenerator)
{
    //Save the generator and parent.
    m_viewerGenerator = viewerGenerator;
}

KNMailWebViewerBase *KNMailGlobal::generateWebViewer(QWidget *parent)
{
    //Check the generator.
    return m_webViewerGenerator==nullptr?
                //Return NULL for not set the generator.
                nullptr:
                //Generate the web viewer.
                m_webViewerGenerator->generateWebViewer(parent);
}
