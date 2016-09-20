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

#include "knmailaccount.h"
#include "knmailaccountmanager.h"
#include "knmailprotocolmanager.h"
#include "knmailreceiverprotocol.h"

#include "knmailreceivermanager.h"

#include <QDebug>

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

void KNMailReceiverManager::updateAllAccount()
{
    //Check working state.
    if(m_isWorking)
    {
        //It is updating all the accounts.
        return;
    }
    //Append all the account pointer to the queue.
    m_updateQueue.append(knMailAccountManager->accountList());
    //Check the working states.
    m_workingLock.lock();
    //Check the working state.
    if(!m_isWorking)
    {
        //Update the working flag.
        m_isWorking=true;
        //Emit the signal.
        emit requireUpdateNext();
    }
    //Release the lock.
    m_workingLock.unlock();
}

void KNMailReceiverManager::appendAccount(int accountIndex)
{
    //Append all the account pointer to the queue.
    m_updateQueue.append(knMailAccountManager->account(accountIndex));
    //Check working state.
    if(m_isWorking)
    {
        //It is updating all the accounts.
        return;
    }
    //Check the working states.
    m_workingLock.lock();
    //Check the working state.
    if(!m_isWorking)
    {
        //Update the working flag.
        m_isWorking=true;
        //Emit the signal.
        emit requireUpdateNext();
    }
    //Release the lock.
    m_workingLock.unlock();
}

void KNMailReceiverManager::onUpdateNextItem()
{
    //Check the queue state.
    if(m_updateQueue.isEmpty())
    {
        //Lock and change the state.
        m_workingLock.lock();
        //Update the flag.
        m_isWorking=false;
        //Release the lock
        m_workingLock.unlock();
        //Mission complete.
        return;
    }
    //Get the first item in the queue.
    KNMailAccount *account=m_updateQueue.takeFirst();
    //Check the validation.
    if(account->isValid())
    {
        qDebug()<<account->username()<<account->receiveProtocolName();
        //Generate the protocol.
        m_workingProtocol.reset(
                    knMailProtocolManager->generateReceiverProtocol(
                        account->receiveProtocolName()));
        qDebug()<<m_workingProtocol.isNull();
        //Null pointer checking.
        if(m_workingProtocol.isNull())
        {
            //! FIXME: Emit a notification to notice that wrong configuration.
        }
        else
        {
            qDebug()<<"Start to update account.";
            //Set the working account to the protocol.
            m_workingProtocol->setAccount(account);
            //Update all the folder content.
            m_workingProtocol->updateFolderStatus();
            qDebug()<<"Complete.";
            //Reset the null.
            m_workingProtocol.reset(nullptr);
        }
    }
    //Ask to process next.
    emit requireUpdateNext();
}

KNMailReceiverManager::KNMailReceiverManager(QObject *parent) :
    QObject(parent),
    m_isWorking(false)
{
    //Reset the pointer.
    m_workingProtocol.reset(nullptr);
    //Link the request loop.
    connect(this, &KNMailReceiverManager::requireUpdateNext,
            this, &KNMailReceiverManager::onUpdateNextItem,
            Qt::QueuedConnection);
    connect(knMailAccountManager, &KNMailAccountManager::accountAppended,
            this, &KNMailReceiverManager::appendAccount,
            Qt::QueuedConnection);
}
