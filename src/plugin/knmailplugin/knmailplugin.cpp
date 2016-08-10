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

//Ports.
#include "knmailemptyhintbase.h"
#include "knmailfolderviewerbase.h"
#include "knmailviewerbase.h"

//Plugins.
// Empty hint.
#include "plugin/knmailemptyhint/knmailemptyhint.h"
// Folder viewer.
#include "plugin/knmailfolderviewer/knmailfolderviewer.h"
// Mail viewer.
#include "plugin/knmailviewer/knmailviewer.h"
// Web viewers.
#ifdef BACKEND_WEBENGINE
#include "plugin/knmailwebengineviewer/knmailwebengineviewer.h"
#endif
#ifdef BACKEND_WEBKIT
#include "plugin/knmailwebkitviewer/knmailwebkitviewer.h"
#endif

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
    //Load the empty hint.
    loadEmptyHint(new KNMailEmptyHint);
    //Load the folder viewer.
    loadFolderViewer(new KNMailFolderViewer);

    //Debug
    m_mainLayout->setCurrentIndex(1);
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
    account->setDisplayName("Haolei Ye");
    account->setUsername("abc@anu.edu.au");
    account->setProvider("netease");
    knMailAccountManager->appendAccount(account);

    account=new KNMailAccount(this);
    account->setDisplayName("JinShuai Ma");
    account->setUsername("def@anu.edu.au");
    account->setProvider("netease");
    knMailAccountManager->appendAccount(account);
}

void KNMailPlugin::onPopupMail(const QString &mailPath)
{
    //Generate the viewer.
    KNMailViewerBase *viewer=generateViewer();
    //Set the viewer parent.
    viewer->setParent(this);
    //Update the viewer state.
    viewer->setViewerPopup(true);
    viewer->setPopupButtonEnabled(false);
    //! FIXME: Add apply mail path here.
    //Show the viewer.
    viewer->show();
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

inline KNMailViewerBase *KNMailPlugin::generateViewer()
{
    //Generate a viewer.
    KNMailViewerBase *viewer=new KNMailViewer(generateWebViewer(nullptr));
    //Return the viewer.
    return viewer;
}

inline KNMailWebViewerBase *KNMailPlugin::generateWebViewer(QWidget *parent)
{
#ifdef BACKEND_WEBENGINE
    return new KNMailWebEngineViewer(parent);
#elif BACKEND_WEBKIT
    return new KNMailWebkitViewer(parent);
#else
    return nullptr;
#endif
}

void KNMailPlugin::loadEmptyHint(KNMailEmptyHintBase *emptyHint)
{
    //Add widget to the stacked layout.
    m_mainLayout->addWidget(emptyHint);
}

void KNMailPlugin::loadFolderViewer(KNMailFolderViewerBase *folderViewer)
{
    //Generate the mail viewer.
    KNMailViewerBase *mailViewer=generateViewer();
    //Link the mail viewer to this plugin.
    connect(mailViewer, &KNMailViewerBase::requirePopup,
            this, &KNMailPlugin::onPopupMail);
    //Give the mail viewer to folder viewer.
    folderViewer->setViewer(mailViewer);
    //Add widget to the stacked layout.
    m_mainLayout->addWidget(folderViewer);
    //Connect with the mail account list.
    connect(m_leftBarContainer, &KNMailAccountList::requireShowFolder,
            folderViewer, &KNMailFolderViewerBase::setFolderModel);
}
