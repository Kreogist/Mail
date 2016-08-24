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
#include "knmailcomposebase.h"

#include "knmailcomposermanager.h"

KNMailComposerManager *KNMailComposerManager::m_instance=nullptr;

KNMailComposerManager::~KNMailComposerManager()
{
    //Clear the button.
    for(auto i : m_composerList)
    {
        //Close the button.
        i->close();
        //Delete the window.
        delete i;
    }
    //Clear the list.
    m_composerList.clear();
}

KNMailComposerManager *KNMailComposerManager::instance()
{
    return m_instance;
}

void KNMailComposerManager::initial(QObject *parent)
{
    //Construct the instance.
    if(m_instance==nullptr)
    {
        //Construct the instance.
        m_instance=new KNMailComposerManager(parent);
    }
}

void KNMailComposerManager::append(KNMailComposeBase *composer)
{
    //Add to the list.
    m_composerList.append(composer);
    //Link the close event.
    connect(composer, &KNMailComposeBase::aboutToClose,
            this, &KNMailComposerManager::onComposeWindowClose);
}

void KNMailComposerManager::onComposeWindowClose()
{
    //Cast the compose window.
    KNMailComposeBase *composer=static_cast<KNMailComposeBase *>(sender());
    //Remove the composer in list.
    m_composerList.removeAll(composer);
}

KNMailComposerManager::KNMailComposerManager(QObject *parent) : QObject(parent)
{
}
