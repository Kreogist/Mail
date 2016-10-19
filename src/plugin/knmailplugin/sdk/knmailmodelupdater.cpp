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
#include <QScopedPointer>
#include <QThread>

#include "knmailaccount.h"
#include "knmailmodel.h"
#include "knmailreceiverprotocol.h"
#include "knmailprotocolmanager.h"

#include "knmailmodelupdater.h"

#include <QDebug>

KNMailModelUpdater *KNMailModelUpdater::m_instance=nullptr;

KNMailModelUpdater::KNMailModelUpdater(QObject *parent) :
    QObject(parent),
    m_queueInsert(false),
    m_isWorking(false)
{
    connect(this, &KNMailModelUpdater::processNext,
            this, &KNMailModelUpdater::onProcessNext,
            Qt::QueuedConnection);

    connect(this, &KNMailModelUpdater::processNextList,
            this, &KNMailModelUpdater::onProcessNextList,
            Qt::QueuedConnection);
}

KNMailModelUpdater *KNMailModelUpdater::instance()
{
    return m_instance;
}

void KNMailModelUpdater::initial(QThread *workingThread)
{
    //Check the pointer.
    if(m_instance==nullptr)
    {
        //Initial the instance.
        m_instance=new KNMailModelUpdater();
        //Move to working thread.
        m_instance->moveToThread(workingThread);
        //Start working.
        workingThread->start();
    }
}

void KNMailModelUpdater::startUpdateFolder(KNMailAccount *account,
                                           KNMailModel *folder,
                                           int startPosition,
                                           int endPosition)
{
    qDebug()<<"Start update folder.";
    endPosition=qMin(folder->rowCount(),
                     endPosition);
    //Search in the list.
    for(int i=0; i<m_updateQueue.size(); ++i)
    {
        const ModelUpdateItem &updateItem=m_updateQueue.at(i);
        if(updateItem.account==account &&
                updateItem.folder==folder &&
                updateItem.startPosition==startPosition &&
                updateItem.endPosition==endPosition)
        {
            //Find the same request, ignore it.
            return;
        }
    }
    //Create the item.
    ModelUpdateItem updateItem;
    //Save the update item data.
    updateItem.account=account;
    updateItem.folder=folder;
    updateItem.current=startPosition;
    updateItem.startPosition=startPosition;
    updateItem.endPosition=endPosition;
    //Add to the beginning of the queue.
    m_updateQueue.prepend(updateItem);
    //Update the flag.
    if(m_isWorking)
    {
        //Change the queue insert flag.
        m_queueInsert=true;
    }
    else
    {
        //Set the flag.
        m_isWorking=true;
        //Start working.
        emit processNext();
    }
}

void KNMailModelUpdater::startUpdateFolderList(KNMailAccount *account,
                                               KNMailModel *folder)
{
    ;
}

void KNMailModelUpdater::onProcessNext()
{
    //Check the queue.
    if(m_updateQueue.isEmpty())
    {
        //Set the flag.
        m_isWorking=false;
        //Finished.
        return;
    }
    //Get the item.
    ModelUpdateItem updateItem=m_updateQueue.takeFirst();
    //Get the receive protocol.
    QScopedPointer<KNMailReceiverProtocol> receiveProtocol(
                knMailProtocolManager->generateReceiverProtocol(
                    updateItem.account->receiveProtocolName()));
    //Configure the protocol.
    receiveProtocol->setAccount(updateItem.account);
    //Loop and update the folder.
    for(int i=updateItem.current; i<updateItem.endPosition; ++i)
    {
        //Check item is cached or not.
        if(!updateItem.folder->isItemCached(i))
        {
            //Update the folder content.
            receiveProtocol->downloadMail(updateItem.folder, i);
        }
        //Change the current to next.
        updateItem.current=i+1;
        //Check the insert state.
        if(m_queueInsert)
        {
            qDebug()<<"Break!";
            //Reset the flag.
            m_queueInsert=false;
            //Append the request to the end.
            m_updateQueue.append(updateItem);
            //Break the loop.
            break;
        }
    }
    //Do next.
    emit processNext();
}

void KNMailModelUpdater::onProcessNextList()
{
    //Check the queue.
    if(m_listUpdateQueue.isEmpty())
    {
        //Mission complete.
        return;
    }
    //Update the folder.
    ModelUpdateList updateItem=m_listUpdateQueue.takeFirst();
    //Get the receive protocol.
    QScopedPointer<KNMailReceiverProtocol> receiveProtocol(
                knMailProtocolManager->generateReceiverProtocol(
                    updateItem.account->receiveProtocolName()));
    //Configure the protocol.
    receiveProtocol->setAccount(updateItem.account);
    //Update folder.
    receiveProtocol->updateFolder(updateItem.folder);
    //Do next.
    emit processNextList();
}
