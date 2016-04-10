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

//Dependences.
#include "kmglobal.h"
#include "knconfiguremanager.h"
#include "kmmainwindow.h"
#include "knversion.h"

//Ports.
#include "kmtitlebarbase.h"
#include "kmleftbarbase.h"
#include "kmmailcomponentbase.h"

#include "kmmaillistmodel.h"

//Plugins.
#include "plugins/kmtitlebar/kmtitlebar.h"
#include "plugins/kmleftbar/kmleftbar.h"
#include "plugins/kmmailcomponent/kmmailcomponent.h"

#include "kmpluginmanager.h"

KMPluginManager::KMPluginManager(QObject *parent) :
    QObject(parent),
    m_mainWindow(nullptr)
{
    //Set the application information.
    setApplicationInformation();
    //Set fusion style to application.
    QApplication::setStyle(QStyleFactory::create("fusion"));
    //Initial the global.
    KMGlobal::initial(this);
}

KMPluginManager::~KMPluginManager()
{
    //Save the configure.
    knConf->saveConfigure();
}

KMMainWindow *KMPluginManager::mainWindow() const
{
    return m_mainWindow;
}

void KMPluginManager::setMainWindow(KMMainWindow *mainWindow)
{
    if(m_mainWindow==nullptr)
    {
        //Save the main window object.
        m_mainWindow = mainWindow;
        //Set the main window to global.
        kmGlobal->setMainWindow(m_mainWindow);
    }
}

void KMPluginManager::loadPlugins()
{
    //Load title bar plugin.
    loadTitleBar(new KMTitleBar);
    //Load left bar plugin.
    loadLeftBar(new KMLeftBar);
    //Load mail component plugin.
    loadMailComponent(new KMMailComponent);
}

void KMPluginManager::launchApplication()
{
    //Start working.
    kmGlobal->startReceiverManager();
    //Show the main window.
    m_mainWindow->show();
}

void KMPluginManager::onActionArgumentsAvaliable(QStringList arguments)
{
    //If there's only one item in arguments list, it means no arguments get
    //from system.
    if(arguments.size()<2)
    {
        return;
    }
    //Remove the first item in the arguments list.
    //That's the file path of the application execution.
    arguments.removeFirst();
    //! FIXME: Process the arguments.
}

void KMPluginManager::loadTitleBar(KMTitleBarBase *titleBar)
{
    //Check pointer first.
    if(!titleBar)
    {
        //Ignore the title bar poitner if the pointer is nullptr.
        return;
    }
    //Load the title bar to main window.
    m_mainWindow->setTitleBar(titleBar);
}

void KMPluginManager::loadLeftBar(KMLeftBarBase *leftBar)
{
    //Check pointer first.
    if(!leftBar)
    {
        //Ignore the pointer if the pointer is null.
        return;
    }
    //Load the left bar to main window, which is the mail list.
    m_mainWindow->setMailList(leftBar);

    KMMailListModel *model=new KMMailListModel(this);
    MailUtil::MailListItem item;
    item.title="TechLauncher: First peer assessment is due at 17:00 today";
    item.sender="Shayne Filnt";
    item.breifContext="Hi, a quick reminder that your Week 4 peer assessment is due";
    for(int i=0; i<30; ++i)
    {
        model->appendRow(item);
    }
    leftBar->setMailListModel(model);
}

void KMPluginManager::loadMailComponent(KMMailComponentBase *mailComponent)
{
    //Check the pointer first.
    if(!mailComponent)
    {
        //Ignore the pointer if the pointer is null.
        return;
    }
    //Load the mail component to main window, which is the mail list.
    m_mainWindow->setMailComponent(mailComponent);
}

inline void KMPluginManager::setApplicationInformation()
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

