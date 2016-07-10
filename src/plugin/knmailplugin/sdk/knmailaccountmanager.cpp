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
#include "knmailaccount.h"

#include "knmailaccountmanager.h"

KNMailAccountManager *KNMailAccountManager::m_instance=nullptr;

KNMailAccountManager::KNMailAccountManager(QObject *parent) :
    QObject(parent)
{
}

KNMailAccountManager *KNMailAccountManager::instance()
{
    return m_instance;
}

void KNMailAccountManager::initial(QObject *parent)
{
    //Check the instance pointer first.
    if(m_instance==nullptr)
    {
        //Initial the manager for the first time.
        m_instance=new KNMailAccountManager(parent);
    }
}

void KNMailAccountManager::appendAccount(KNMailAccount *account)
{
    //Change the account relationship.
    account->setParent(this);
    //Add account object to the account list.
    m_accountList.append(account);
    //Emit the extend signal.
    emit accountAppended(m_accountList.size()-1);
}

KNMailAccount *KNMailAccountManager::account(int accountIndex)
{
    return m_accountList.at(accountIndex);
}

int KNMailAccountManager::accountCount()
{
    return m_accountList.size();
}
