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
#include <QJsonArray>
#include <QJsonDocument>

#include "knconfigure.h"
#include "kmglobal.h"
#include "kmmailaccount.h"
#include "crypto/kmcryptoaes.h"

#include "kmmailaccountmanager.h"

#include <QDebug>

#define UniqueKey "org.kreogist.mail.accounts"

KMMailAccountManager *KMMailAccountManager::m_instance=nullptr;

KMMailAccountManager::KMMailAccountManager(QObject *parent) :
    QObject(parent),
    m_userConfigure(kmGlobal->userConfigure()->getConfigure("MailAccounts"))
{
    //Initial property names.
    m_accountProperies[DisplayName]="Display Name";
    m_accountProperies[UserName]="Username";
    m_accountProperies[Password]="Password";
    m_accountProperies[SendProtocol]="Send Protocol";
    m_accountProperies[ReceiverProtocol]="Receiver Protocol";
}

KMMailAccountManager *KMMailAccountManager::instance()
{
    return m_instance;
}

void KMMailAccountManager::initial(QObject *parent)
{
    //Check instance pointer first.
    if(m_instance)
    {
        //Ignore the double constrcution.
        return;
    }
    //Initial the static pointer.
    m_instance=new KMMailAccountManager(parent);
}

void KMMailAccountManager::addMailAccount(KMMailAccount *account)
{
    //Change account parent.
    account->setParent(this);
    //Append account to the account list.
    m_accountList.append(account);
    //Emit the signal to notify account append.
    emit accountAppend(account);
}

void KMMailAccountManager::loadConfigure()
{
    //Recover data from user configure.
    QJsonArray accountList=
            QJsonDocument::fromJson(
                m_userConfigure->data("Accounts").toString().toUtf8()).array();
    //Generate account data and add to mail account.
    for(auto i=accountList.begin(); i!=accountList.end(); ++i)
    {
        //Get the account info object.
        QJsonObject &&accountInfo=(*i).toObject();
        //Generate account object.
        KMMailAccount *account=new KMMailAccount(this);
        //Set account data.
        for(int j=0; j<MailAccountPropertiesCount; ++j)
        {
            //Set the account property.
            account->setAccountProperty(j, accountInfo.value(
                                            m_accountProperies[j]).toString());
        }
        //The account folder shall be allocate.
        account->setDirName(account->accountProperty(UserName));
        //Append account.
        addMailAccount(account);
    }
}

void KMMailAccountManager::saveConfigure()
{
    //Generate a json list for store all the information.
    QJsonArray accountList;
    //Save account list to configure.
    for(auto i=m_accountList.begin(); i!=m_accountList.end(); ++i)
    {
        //Generate a json object for the account.
        QJsonObject accountInfo;
        //Save all the properties.
        for(int j=0; j<MailAccountPropertiesCount; ++j)
        {
            //Save the account information.
            accountInfo.insert(m_accountProperies[j], (*i)->accountProperty(j));
        }
        //Add account info to the list.
        accountList.append(accountInfo);
        //Remove the account list.
        (*i)->saveAccountData();
    }
    //Translate the account list into json string.
    m_userConfigure->setData("Accounts",
                             QString(QJsonDocument(accountList).toJson(
                                         QJsonDocument::Compact)));
    //Clear the account list.
    m_accountList.clear();
}
