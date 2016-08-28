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
#include <QJsonObject>

#include "knconfigure.h"
#include "knglobal.h"
#include "knmailutil.h"

#include "knmailaccount.h"

#include "knmailaccountmanager.h"

#define ConfigureName "MailAccounts"
#define FieldUsername "Username"
#define FieldPassword "Password"
#define FieldDisplayName "Display Name"
#define FieldProvider "Provider"
#define FieldSendProtocol "Send Protocol"
#define FieldSendConfig "Send Config"
#define FieldReceiveProtocol "Receive Protocol"
#define FieldReceiveConfig "Receive Config"
#define FieldConfigHostName "Host Name"
#define FieldConfigPort "Port"
#define FieldConfigLogin "Login Format"
#define FieldConfigSslVersion "Ssl Version"
#define FieldConfigSocketType "Socket Type"

KNMailAccountManager *KNMailAccountManager::m_instance=nullptr;

KNMailAccountManager::KNMailAccountManager(QObject *parent) :
    QAbstractListModel(parent),
    m_accountConfigure(knGlobal->userConfigure()->getConfigure(ConfigureName))
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

int KNMailAccountManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    //Account count is the list count.
    return m_accountList.size();
}

QVariant KNMailAccountManager::data(const QModelIndex &index, int role) const
{
    //Check the validation of the index.
    if(!index.isValid())
    {
        //No useful data will be returned.
        return QVariant();
    }
    //Get the index row data.
    const KNMailAccount *account=m_accountList.at(index.row());
    //Check the account information.
    switch(role)
    {
    case Qt::DisplayRole:
        return QString("%1 <%2>").arg(account->displayName(),
                                      account->username());
    default:
        return QVariant();
    }
}

void KNMailAccountManager::loadAccountList()
{
    //Load the account from the configuration.
    QJsonArray accountList=
            QJsonDocument::fromJson(
                m_accountConfigure->data("Accounts").toString().toUtf8()
                ).array();
    //Load data from account list.
    for(auto i : accountList)
    {
        //Generate a mail account.
        KNMailAccount *mailAccount=new KNMailAccount(this);
        //Get the account information object.
        QJsonObject &&account=i.toObject();
        //Parse the account information.
        mailAccount->setUsername(account.value(FieldUsername).toString());
        mailAccount->setPassword(account.value(FieldPassword).toString());
        mailAccount->setDisplayName(account.value(FieldDisplayName).toString());
        mailAccount->setProvider(account.value(FieldProvider).toString());
        mailAccount->setReceiveProtocolName(
                    account.value(FieldSendProtocol).toString());
        mailAccount->setSendProtocolName(
                    account.value(FieldReceiveProtocol).toString());
        QJsonObject configObject=account.value(FieldSendConfig).toObject();
        mailAccount->setSendConfig(toConfig(&configObject));
        configObject=account.value(FieldReceiveConfig).toObject();
        mailAccount->setReceiveConfig(toConfig(&configObject));
        //Append account to list.
        appendAccount(mailAccount);
    }
}

void KNMailAccountManager::saveAccountList()
{
    //Load the account from the configuration.
    QJsonArray accountList;
    //Load data from account list.
    for(auto mailAccount : m_accountList)
    {
        //Save the mail account folders.
        mailAccount->saveFolder();
        //Get the account information object.
        QJsonObject account;
        //Parse the account information.
        account.insert(FieldUsername, mailAccount->username());
        account.insert(FieldPassword, mailAccount->password());
        account.insert(FieldDisplayName, mailAccount->displayName());
        account.insert(FieldProvider, mailAccount->provider());
        account.insert(FieldSendProtocol, mailAccount->receiveProtocolName());
        account.insert(FieldReceiveProtocol, mailAccount->sendProtocolName());
        KNMailProtocolConfig config=mailAccount->sendConfig();
        account.insert(FieldSendConfig, fromConfig(&config));
        config=mailAccount->receiveConfig();
        account.insert(FieldReceiveConfig, fromConfig(&config));
        //Append account to list.
        accountList.append(account);
    }
    //Write content
    m_accountConfigure->setData("Accounts",
                                QString(QJsonDocument(accountList).toJson(
                                            QJsonDocument::Compact)));
}

inline KNMailProtocolConfig KNMailAccountManager::toConfig(
        QJsonObject *configObject)
{
    //Initial the config.
    KNMailProtocolConfig protocolConfig;
    //Set the data.
    protocolConfig.hostName=configObject->value(FieldConfigHostName).toString();
    protocolConfig.port=configObject->value(FieldConfigPort).toInt();
    protocolConfig.loginFormat=configObject->value(FieldConfigLogin).toString();
    protocolConfig.sslVersion=(QSsl::SslProtocol)configObject->value(
                FieldConfigSslVersion).toInt();
    protocolConfig.socketType=(MailProtocolSocket)configObject->value(
                FieldConfigSocketType).toInt();
    //Give back the procotol config.
    return protocolConfig;
}

inline QJsonObject KNMailAccountManager::fromConfig(
        KNMailProtocolConfig *protocolConfig)
{
    //Initial the object.
    QJsonObject configObject;
    //Save the data.
    configObject.insert(FieldConfigHostName, protocolConfig->hostName);
    configObject.insert(FieldConfigPort, protocolConfig->port);
    configObject.insert(FieldConfigLogin, protocolConfig->loginFormat);
    configObject.insert(FieldConfigSslVersion, (int)protocolConfig->sslVersion);
    configObject.insert(FieldConfigSocketType, (int)protocolConfig->socketType);
    //Give back the config object.
    return configObject;
}
