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
#include <QThread>

#include "knmailreceivermanager.h"

KNMailReceiverManager *KNMailReceiverManager::m_instance=nullptr;

KNMailReceiverManager *KNMailReceiverManager::instance()
{
    return m_instance;
}

void KNMailReceiverManager::initial(QThread *workingThread)
{
    //Check the instance pointer.
    if(!m_instance)
    {
        //Construct the instance.
        m_instance=new KNMailReceiverManager();
        //Move to working thread.
        m_instance->moveToThread(workingThread);
        //Start the working thread.
        workingThread->start();
    }
}

KNMailReceiverManager::KNMailReceiverManager(QObject *parent) :
    QObject(parent)
{

}
