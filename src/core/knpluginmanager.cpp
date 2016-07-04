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

//Ports


#include "knpluginmanager.h"

KNPluginManager::KNPluginManager(QObject *parent) :
    QObject(parent)
{
    //Set the application information.
    setApplicationInformation();
    //Set fusion style to application.
    QApplication::setStyle(QStyleFactory::create("fusion"));
    //Initial the global.
    KNGlobal::initial(this);
}

void KNPluginManager::setMainWindow(KNMainWindow *mainWindow)
{
    //Set the main window to global.
    knGlobal->setMainWindow(mainWindow);
}

void KNPluginManager::loadPlugins()
{
    //Initial the main window infrasturctures.
    ;
}

void KNPluginManager::launchApplication()
{
    ;
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
    //Get the main window.
    KNMainWindow *mainWindow=knGlobal->mainWindow();
    ;
}

void KNPluginManager::loadEmptyHint(KNMainWindowEmptyHintBase *emptyHint)
{
    ;
}
