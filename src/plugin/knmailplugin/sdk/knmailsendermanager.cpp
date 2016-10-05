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
#include "mime/knmimepart.h"
#include "knmailaccount.h"
#include "knmailprotocolmanager.h"
#include "knmailsenderprotocol.h"

#include "knmailsendermanager.h"

#include <QDebug>

KNMailSenderManager *KNMailSenderManager::m_instance=nullptr;

void KNMailSenderManager::initial(QThread *workingThread)
{
    if(m_instance==nullptr)
    {
        m_instance=new KNMailSenderManager();
        m_instance->moveToThread(workingThread);
    }
}

KNMailSenderManager *KNMailSenderManager::instance()
{
    return m_instance;
}

void KNMailSenderManager::sendMail(KNMimePart *mimePart,
                                   KNMailAccount *account,
                                   const QString &receiver)
{
    qDebug()<<"Fuck?!";
    //Construct the item
    SendMailItem item;
    item.account=account;
    item.mimePart=mimePart;
    item.receiver=receiver;
    //Add to list.
    m_mailItemList.append(item);
    //Check the working state.
    if(m_isWorking)
    {
        return;
    }
    //Start to work.
    m_isWorking=true;
    emit requireUpdateNext();
}

void KNMailSenderManager::onUpdateNextItem()
{
    qDebug()<<"Start to work!";
    //Check the queue state.
    if(m_mailItemList.isEmpty())
    {
        //No more working.
        m_isWorking=false;
        return;
    }
    //Update working state.
    m_isWorking=true;
    //Get the item in the list.
    SendMailItem item=m_mailItemList.takeFirst();
    //Send the item.
    //Get the protocol.
    QScopedPointer<KNMailSenderProtocol> m_workingProtocol;
    m_workingProtocol.reset(knMailProtocolManager->generateSenderProtocol(
                                item.account->sendProtocolName()));
    qDebug()<<"Is null?"<<m_workingProtocol.isNull();
    //Null pointer checking.
    if(m_workingProtocol.isNull())
    {
        //Failed.
    }
    else
    {
        //Set the mail account.
        m_workingProtocol->setAccount(item.account);
        //Start to send the mail.
        m_workingProtocol->sendMail(item.receiver,
                                    item.mimePart);
        //Recover the mime part.
        delete item.mimePart;
        //Reset the protocol.
        m_workingProtocol.reset(nullptr);
    }
    //Ask to process next.
    emit requireUpdateNext();
}

KNMailSenderManager::KNMailSenderManager(QObject *parent) :
    QObject(parent),
    m_isWorking(false)
{
    //Connect the manager.
    connect(this, &KNMailSenderManager::requireUpdateNext,
            this, &KNMailSenderManager::onUpdateNextItem,
            Qt::QueuedConnection);
}
