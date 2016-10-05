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
#include <QPushButton>

//Dependencies.
#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knglobal.h"
#include "knmainwindow.h"
#include "knlinearsensewidget.h"
#include "knopacityanimebutton.h"

//Mail plugin sdks.
#include "knmailglobal.h"
#include "knmailaccountlist.h"
#include "knmailpopupmanager.h"
#include "knmailcomposermanager.h"
#include "knmailprotocolmanager.h"
#include "knmailaccountadddialog.h"

//Ports.
#include "knmailemptyhintbase.h"
#include "knmailfolderviewerbase.h"
#include "knmailcomposegeneratorbase.h"
#include "knmailcomposebase.h"
#include "knmailviewerbase.h"
#include "knmailviewergeneratorbase.h"
#include "knmailwebviewergeneratorbase.h"
#include "knmailprotocol.h"
#include "knmailprotocolgenerator.h"
#include "knmailcomposebase.h"

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
// Composer.
#include "plugin/knmailcompose/knmailcompose.h"
#include "plugin/knmailcompose/knmailcomposegenerator.h"
// Protocols.
#include "plugin/knmailimapprotocol/knmailimapprotocolgenerator.h"
#include "plugin/knmailsmtpprotocol/knmailsmtpprotocolgenerator.h"

#include "knmailplugin.h"

//Debug
#include <QDebug>
#include "mime/knmimepart.h"
#include "knmailaccount.h"
#include "knmailaccountmanager.h"

KNMailPlugin::KNMailPlugin(QWidget *parent) :
    KNMailPluginBase(parent),
    m_leftBarContainer(nullptr),
    m_addAccount(new KNMailAccountAddDialog(knGlobal->mainWindow())),
    m_bottomBar(nullptr),
    m_composeButton(new QPushButton(this)),
    m_mainLayout(new QStackedLayout(this))
{
    //Initial the basic infrastructure.
    initialInfrastructure();
    //Link the signal with main window.
    connect(knGlobal->mainWindow(), &KNMainWindow::aboutToClose,
            knMailPopupManager, &KNMailPopupManager::closeAllMail);
    //Link the retranslator.
    connect(knI18n, &KNLocaleManager::languageChange,
            this, &KNMailPlugin::retranslate);
    retranslate();
}

QWidget *KNMailPlugin::accountPanel()
{
    return m_leftBarContainer;
}

QWidget *KNMailPlugin::bottomBar()
{
    return m_bottomBar;
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
    //Load the composer generator.
    loadComposerGenerator(new KNMailComposeGenerator);
    //Load the empty hint.
    loadEmptyHint(new KNMailEmptyHint);
    //Load the folder viewer.
    loadFolderViewer(new KNMailFolderViewer);
    //Load the protocol factorys.
    knMailProtocolManager->appendFactory(new KNMailImapProtocolGenerator);
    knMailProtocolManager->appendFactory(new KNMailSmtpProtocolGenerator);

    //Load the account information.
    knMailAccountManager->loadAccountList();

    //Debug
    m_mainLayout->setCurrentIndex(1);
}

inline void KNMailPlugin::initialInfrastructure()
{
    //Initial the mail plugin global.
    KNMailGlobal::initial(this);

    //Load the left bar container.
    m_leftBarContainer=new KNMailAccountList(this);
    //Configure the layout.
    m_mainLayout->setContentsMargins(7, 0, 7, 0);
    setLayout(m_mainLayout);
    //Configure the button.
    m_composeButton->setContentsMargins(7, 0, 7, 0);
    m_composeButton->setObjectName("MailComposeButton");
    knTheme->registerWidget(m_composeButton);
    m_composeButton->setFixedHeight(28);
    m_composeButton->setIcon(QIcon("://public/create.png"));
    //Link the compose button to generator.
    connect(m_composeButton, &QPushButton::released,
            this, &KNMailPlugin::onActionComposePressed);

    //Initial the bottom bar.
    m_bottomBar=new KNLinearSenseWidget(this);
    m_bottomBar->setFixedHeight(30);
    //Create the bottom bar layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    buttonLayout->setContentsMargins(7, 2, 7, 2);
    buttonLayout->setSpacing(7);
    m_bottomBar->setLayout(buttonLayout);
    //Create add button and remove button.
    KNOpacityAnimeButton *addButton=new KNOpacityAnimeButton(this);
    addButton->setFixedSize(16, 16);
    addButton->setIcon(QIcon(":/plugin/mail/account/editor_add.png"));
    buttonLayout->addWidget(addButton);
    buttonLayout->addStretch();
    //Link the add button to show the dialog.
    connect(addButton, &KNOpacityAnimeButton::clicked,
            [=]{m_addAccount->show();});
}

void KNMailPlugin::loadEmptyHint(KNMailEmptyHintBase *emptyHint)
{
    //Add widget to the stacked layout.
    m_mainLayout->addWidget(emptyHint);
}

void KNMailPlugin::loadFolderViewer(KNMailFolderViewerBase *folderViewer)
{
    //Generate the mail viewer.
    KNMailViewerBase *mailViewer=knMailGlobal->generateViewer();
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
    knMailGlobal->setViewerGenerator(generator);
}

void KNMailPlugin::loadComposerGenerator(KNMailComposeGeneratorBase *generator)
{
    //Set the generator.
    knMailGlobal->setComposerGenerator(generator);
}

QWidget *KNMailPlugin::composeButton() const
{
    return m_composeButton;
}

void KNMailPlugin::saveAccount()
{
    //Save the content data.
    knMailAccountManager->saveAccountList();
}

void KNMailPlugin::startWorking()
{
    //Emit the global update all signal.
    emit knMailGlobal->requireUpdateAll();
}

void KNMailPlugin::onArgumentsAvaliable(const QStringList &arguments)
{
    //Treat the arguments as the file, we will open them to show the content of
    //the mail.
    ;
}

void KNMailPlugin::retranslate()
{
    //Update the compose button.
    m_composeButton->setText(" "+tr("Compose"));
}

void KNMailPlugin::onActionComposePressed()
{
    //Generate a new composer.
    KNMailComposeBase *composer=knMailGlobal->generateComposer();
    //Add to manager.
    knMailComposerManager->append(composer);
    //Show the composer.
    composer->show();
    //Raise the composer.
    composer->raise();
    composer->activateWindow();
}
