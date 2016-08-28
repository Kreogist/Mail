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
#include "knmailreceiverprotocol.h"
#include "knmailprotocolgenerator.h"

#include "knmailprotocolmanager.h"

KNMailProtocolManager *KNMailProtocolManager::m_instance=nullptr;

KNMailProtocolManager::~KNMailProtocolManager()
{
    //Remove all the content in the map.
    for(auto i : m_receiverMap)
    {
        //Remove the pointer i.
        delete i;
    }
}

KNMailProtocolManager *KNMailProtocolManager::intance()
{
    return m_instance;
}

void KNMailProtocolManager::initial()
{
    //Check the pointer.
    if(!m_instance)
    {
        //Initial the pointer.
        m_instance=new KNMailProtocolManager();
    }
}

KNMailReceiverProtocol *KNMailProtocolManager::generateReceiverProtocol(
        const QString &name)
{
    //Get the generator.
    KNMailProtocolGenerator *factory=m_receiverMap.value(name, nullptr);
    //Check the factory pointer.
    if(factory)
    {
        //Generate the object.
        return static_cast<KNMailReceiverProtocol *>(
                    factory->generateProtocol());
    }
    //A null object will be return.
    return nullptr;
}

void KNMailProtocolManager::appendFactory(KNMailProtocolGenerator *generator)
{
    //Append the generator to the map.
    m_receiverMap.insert(generator->protocolName(), generator);
}

KNMailProtocolManager::KNMailProtocolManager(QObject *parent) :
    QObject(parent)
{
}
