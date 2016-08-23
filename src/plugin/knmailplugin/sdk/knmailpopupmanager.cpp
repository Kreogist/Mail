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
 * along with this program; if not, write to the Free Software
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "knmailglobal.h"

#include "knmailviewerbase.h"

#include "knmailpopupmanager.h"

#include <QDebug>

KNMailPopupManager *KNMailPopupManager::m_instance=nullptr;

void KNMailPopupManager::initial(QObject *parent)
{
    //Check the pointer.
    if(m_instance==nullptr)
    {
        //Create the instance.
        m_instance=new KNMailPopupManager(parent);
    }
}

KNMailPopupManager::KNMailPopupManager(QObject *parent) :
    QObject(parent)
{
}

KNMailPopupManager *KNMailPopupManager::instance()
{
    return m_instance;
}

KNMailViewerBase *KNMailPopupManager::window(const QString &filePath)
{
    //Search the file path in the list.
    return m_pathWindowList.value(filePath, nullptr);
}

void KNMailPopupManager::appendWindow(const QString &filePath,
                                      KNMailViewerBase *window)
{
    //Append the window pointer to the list.
    m_pathWindowList.insert(filePath, window);
    //Link the window to the remove function.
    connect(window, &KNMailViewerBase::requireClose,
            this, &KNMailPopupManager::removeWindow);
}

void KNMailPopupManager::popupMail(const QString &filePath)
{
    //Check in the list.
    KNMailViewerBase *viewerWindow=window(filePath);
    //Check if we find or not.
    if(viewerWindow)
    {
        //Show the window.
        viewerWindow->raise();
        viewerWindow->activateWindow();
        //Mission complete.
        return;
    }
    //We have to generate and show the window.
    viewerWindow=knMailGlobal->generatePopupViewer();
    //Set the file path.
    viewerWindow->loadMail(filePath);
    //Add the viewer into list.
    appendWindow(filePath, viewerWindow);
    //Show and active the window.
    viewerWindow->raise();
    viewerWindow->activateWindow();
    viewerWindow->show();
}

void KNMailPopupManager::closeAllMail()
{
    //Close all the window.
    for(auto i : m_pathWindowList)
    {
        //Close the window.
        delete i;
    }
}

void KNMailPopupManager::removeWindow(const QString &filePath)
{
    //Remove the item from the window list.
    m_pathWindowList.remove(filePath);
}
