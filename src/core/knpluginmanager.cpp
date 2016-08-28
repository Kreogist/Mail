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
#include <QApplication>
#include <QStyleFactory>

//Dependenices.
#include "knversion.h"
#include "knglobal.h"
#include "knmainwindow.h"
#include "knconfiguremanager.h"
#include "account/knaccountwidget.h"

//Ports
#include "knmainwindowleftbarbase.h"
#include "knmailpluginbase.h"

//Plugins
// Left Bar.
#include "knmainwindowleftbar.h"
// Mail Plugin.
#include "plugin/knmailplugin/knmailplugin.h"

#include "knpluginmanager.h"

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent),
    m_leftBar(nullptr)
{
    //Set the application information.
    setApplicationInformation();
    //Set fusion style to application.
    QApplication::setStyle(QStyleFactory::create("fusion"));
    //Initial the global.
    KNGlobal::initial(this);
}

KNPluginManager::~KNPluginManager()
{
    //Save the configure.
    knConf->saveConfigure();
}

void KNPluginManager::setMainWindow(KNMainWindow *mainWindow)
{
    //Set the main window to global.
    knGlobal->setMainWindow(mainWindow);
}

void KNPluginManager::loadPlugins()
{
    //Initial the main window infrasturctures.
    loadLeftBar(new KNMainWindowLeftBar);
    loadMailPlugin(new KNMailPlugin);
}

void KNPluginManager::launchApplication()
{
    //Launch the mail plugin start working function.
    emit requireLaunch();
    //Get the main window pointer.
    KNMainWindow *mainWindow=knGlobal->mainWindow();
    //Show the main window.
    mainWindow->show();
}

void KNPluginManager::onActionArgumentsAvaliable(QStringList arguments)
{
    ;
}

void KNPluginManager::setApplicationInformation()
{
    //Set static application information.
    QApplication::setApplicationName("Mail");
    QApplication::setApplicationVersion(APP_VERSION_STR);
    QApplication::setApplicationDisplayName("Mail");
    QApplication::setOrganizationName("Kreogist Dev Team");
    QApplication::setOrganizationDomain("http://kreogist.github.io/");
    //Configure application attributes.
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
}

void KNPluginManager::loadLeftBar(KNMainWindowLeftBarBase *leftBar)
{
    Q_ASSERT(leftBar);
    //Get the main window.
    KNMainWindow *mainWindow=knGlobal->mainWindow();
    //Save the pointer.
    m_leftBar=leftBar;
    //Add account widget.
    m_leftBar->addLeftBarWidget(new KNAccountWidget(m_leftBar));
    //Set the left bar widget to main window.
    mainWindow->setLeftBar(m_leftBar);
}

void KNPluginManager::loadMailPlugin(KNMailPluginBase *mailPlugin)
{
    Q_ASSERT(mailPlugin);
    //Get the main window.
    KNMainWindow *mainWindow=knGlobal->mainWindow();
    //Load the plugin.
    mailPlugin->loadPlugins();
    //Link the mail plugin.
    connect(this, &KNPluginManager::requireLaunch,
            mailPlugin, &KNMailPluginBase::startWorking);
    //Set the main window widget to main window.
    mainWindow->setMainWidget(mailPlugin);
    //Add the sidebar to left bar.
    m_leftBar->addLeftBarWidget(mailPlugin->composeButton());
    m_leftBar->addLeftBarSpacing(7);
    m_leftBar->addLeftBarWidget(mailPlugin->accountPanel(), 1);
}
