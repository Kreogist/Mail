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
#include <QStackedLayout>

//Dependencies.
#include "knthememanager.h"

//Mail plugin sdks.
#include "knmailglobal.h"
#include "knmailaccountlist.h"
#include "knmailpopupmanager.h"

//Ports.
#include "knmailemptyhintbase.h"
#include "knmailfolderviewerbase.h"
#include "knmailviewerbase.h"
#include "knmailviewergeneratorbase.h"
#include "knmailwebviewergeneratorbase.h"
#include "knmailprotocol.h"

//Plugins.
// Empty hint.
#include "plugin/knmailemptyhint/knmailemptyhint.h"
// Folder viewer.
#include "plugin/knmailfolderviewer/knmailfolderviewer.h"
// Mail viewer.
#include "plugin/knmailviewer/knmailviewer.h"
#include "plugin/knmailviewer/knmailviewergenerator.h"
// Web viewers.
#ifdef BACKEND_WEBENGINE
#include "plugin/knmailwebengineviewer/knmailwebengineviewer.h"
#include "plugin/knmailwebengineviewer/knmailwebengineviewergenerator.h"
#endif
#ifdef BACKEND_WEBKIT
#include "plugin/knmailwebkitviewer/knmailwebkitviewer.h"
#include "plugin/knmailwebkitviewer/knmailwebkitviewergenerator.h"
#endif
// Protocols.
#include "plugin/knmailpop3protocol/knmailpop3protocol.h"
#include "plugin/knmailimapprotocol/knmailimapprotocol.h"

#include "knmailplugin.h"

//Debug
#include <QDebug>
#include "knmailaccount.h"
#include "knmailaccountmanager.h"

KNMailPlugin::KNMailPlugin(QWidget *parent) :
    KNMailPluginBase(parent),
    m_leftBarContainer(nullptr),
    m_mainLayout(new QStackedLayout(this))
{
    //Initial the basic infrastructure.
    initialInfrastructure();
}

QWidget *KNMailPlugin::accountPanel()
{
    return m_leftBarContainer;
}

void KNMailPlugin::loadPlugins()
{
    //Load all the plugins in order.
    //Load the web content generator.
#ifdef BACKEND_WEBENGINE
    loadWebViewerGenerator(new KNMailWebEngineViewerGenerator);
#endif
#ifdef BACKEND_WEBKIT
    loadWebViewerGenerator(new KNMailWebkitViewerGenerator);
#endif
    //Load the mail viewer generator.
    loadMailViewerGenerator(new KNMailViewerGenerator);
    //Load the empty hint.
    loadEmptyHint(new KNMailEmptyHint);
    //Load the folder viewer.
    loadFolderViewer(new KNMailFolderViewer);

    //Debug
    m_mainLayout->setCurrentIndex(1);

    KNMailAccount *superaccount=new KNMailAccount(this);
    superaccount->setDisplayName("Haolei Ye");
    superaccount->setUsername("tomguts@126.com");
    superaccount->setProvider("netease");
    knMailAccountManager->appendAccount(superaccount);

    KNMailAccount *account=new KNMailAccount(this);
    account->setDisplayName("Han Wang");
    account->setUsername("abc@anu.edu.au");
    account->setProvider("google");
    knMailAccountManager->appendAccount(account);

    KNMailModel *inboxFolder=account->folder(FolderInbox);
    KNMailListItem mailItem;
    mailItem.title="Follow Charlie Miller, and Dino A. Dai Zovi on Twitter!";
    mailItem.sender="info@twitter.com";
    mailItem.senderName="Twitter";
    mailItem.receiver="tomguts@126.com";
    mailItem.receiverName="Harinlen";
    mailItem.breifContext="Hey Harinlen Angela, Here are some prople we think you might like to follow.";
    for(int i=0; i<20; ++i)
    {
        inboxFolder->appendRow(mailItem);
    }

    account=new KNMailAccount(this);
    account->setDisplayName("JinShuai Ma");
    account->setUsername("def@anu.edu.au");
    account->setProvider("netease");
    knMailAccountManager->appendAccount(account);

    KNMailImapProtocol *popProtocol=new KNMailImapProtocol(this);
    KNMailProtocolConfig config;
    config.hostName="imap.126.com";
    config.port=993;
    config.socketType=SocketSsl;
    superaccount->setReceiveConfig(config);
    popProtocol->setAccount(superaccount);
    qDebug()<<popProtocol->connectToHost();
    qDebug()<<popProtocol->login();
    qDebug()<<popProtocol->updateFolderStatus();
}

inline void KNMailPlugin::initialInfrastructure()
{
    //Initial the mail plugin global.
    KNMailGlobal::initial(this);

    //Load the left bar container.
    m_leftBarContainer=new KNMailAccountList(this);
    //Configure the layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    setLayout(m_mainLayout);
}

void KNMailPlugin::loadEmptyHint(KNMailEmptyHintBase *emptyHint)
{
    //Add widget to the stacked layout.
    m_mainLayout->addWidget(emptyHint);
}

void KNMailPlugin::loadFolderViewer(KNMailFolderViewerBase *folderViewer)
{
    //Generate the mail viewer.
    KNMailViewerBase *mailViewer=knMailGlobal->generateViewer(folderViewer);
    //Link the mail viewer to this plugin.
    connect(mailViewer, &KNMailViewerBase::requirePopup,
            knMailPopupManager, &KNMailPopupManager::popupMail);
    //Give the mail viewer to folder viewer.
    folderViewer->setViewer(mailViewer);
    //Add widget to the stacked layout.
    m_mainLayout->addWidget(folderViewer);
    //Connect with the mail account list.
    connect(m_leftBarContainer, &KNMailAccountList::requireShowFolder,
            folderViewer, &KNMailFolderViewerBase::setFolderModel);
}

void KNMailPlugin::loadWebViewerGenerator(
        KNMailWebViewerGeneratorBase *generator)
{
    //Set the generator to mail global.
    knMailGlobal->setWebViewerGenerator(generator);
}

void KNMailPlugin::loadMailViewerGenerator(KNMailViewerGeneratorBase *generator)
{
    //Set the generator to the mail global.
    knMailGlobal->setViewerGenerator(generator, this);
}
