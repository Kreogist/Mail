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

#include "kmmailreceivermanager.h"

KMMailReceiverManager *KMMailReceiverManager::m_instance=nullptr;

KMMailReceiverManager *KMMailReceiverManager::instance()
{
    //Give back the instance.
    return m_instance;
}

void KMMailReceiverManager::initial(QObject *parent)
{
    //Check the instance pointer.
    if(m_instance==nullptr)
    {
        //Initial the instance.
        m_instance=new KMMailReceiverManager;
    }
}

void KMMailReceiverManager::appendMail(KNMailAccount *accout)
{
    //Lock the queue.
    m_queueLock.lock();
    //Get the account index in the queue, if it is -1, then it is not in the
    //queue.
    int accountIndex=m_mailRecevieList.indexOf(account);
    //Check whether account is in the list or not.
    if(accountIndex==-1)
    {
        //Append account list.
        m_mailRecevieList.append(accout);
    }
    else
    {
        //Move the account to the top.
        m_mailRecevieList.move(accountIndex, 0);
    }
    //Free the lock.
    m_queueLock.unlock();
    //Check working State.
    if(m_isWorking)
    {
        return;
    }
    //Enabled working.
    m_isWorking = true;
    //Emit the process next signal.
    emit requireProcessNext();
}

void KMMailReceiverManager::onProcessNext()
{
    if(m_mailRecevieList.isEmpty())
    {
        m_isWorking = false;
        return;
    }
    //Request the lock.
    m_queueLock.lock();
    //do something here.
    KNMailAccount *receivedAccount=m_mailRecevieList.takeFirst();
    //Free the lock.
    m_queueLock.unlock();
    //Process next.
    emit requireProcessNext();
}

KMMailReceiverManager::KMMailReceiverManager(QObject *parent) :
    QObject(parent)
{
    connect(this, &KMMailReceiverManager::requireProcessNext,
            this, &KMMailReceiverManager::onProcessNext, Qt::QueuedConnection);

}

