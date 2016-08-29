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

#include "knmailaccount.h"
#include "knmailmodel.h"
#include "knmailreceiverprotocol.h"
#include "knmailprotocolmanager.h"

#include "knmailmodelupdater.h"

#include <QDebug>

KNMailModelUpdater *KNMailModelUpdater::m_instance=nullptr;

KNMailModelUpdater::KNMailModelUpdater(QObject *parent) :
    QObject(parent)
{
}

KNMailModelUpdater *KNMailModelUpdater::instance()
{
    return m_instance;
}

void KNMailModelUpdater::initial(QObject *parent)
{
    //Check the pointer.
    if(m_instance==nullptr)
    {
        //Initial the instance.
        m_instance=new KNMailModelUpdater(parent);
    }
}

void KNMailModelUpdater::updateFolder(KNMailAccount *account,
                                      KNMailModel *folder,
                                      int startPosition,
                                      int endPosition)
{
    qDebug()<<"Start to update folder, protocol"<<account->receiveProtocolName();
    //Get the receive protocol.
    QScopedPointer<KNMailReceiverProtocol> receiveProtocol(
                knMailProtocolManager->generateReceiverProtocol(
                    account->receiveProtocolName()));
    //Configure the protocol.
    receiveProtocol->setAccount(account);
    //Update the folder content.
    receiveProtocol->updateFolderContent(folder, startPosition, endPosition);
}

void KNMailModelUpdater::startUpdateFolder(KNMailAccount *account,
                                           KNMailModel *folder,
                                           int startPosition,
                                           int endPosition)
{
    //Check the current state working state.
    if(m_updateState.isRunning())
    {
        //Quit the update thread.
        m_updateThread.cancel();
    }
    //Launch the future.
    m_updateThread=QtConcurrent::run(this, &KNMailModelUpdater::updateFolder,
                                     account, folder,
                                     startPosition, endPosition);
    //Set the future of the thread.
    m_updateState.setFuture(m_updateThread);
}
