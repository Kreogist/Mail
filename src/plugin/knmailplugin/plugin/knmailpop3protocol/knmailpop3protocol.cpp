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
#include <QTcpSocket>
#include <QSslSocket>

#include "knmailaccount.h"

#include "knmailpop3protocol.h"

KNMailPop3Protocol::KNMailPop3Protocol(QObject *parent) :
    KNMailReceiverProtocol(parent),
    m_breakFlag(false)
{
}

bool KNMailPop3Protocol::connectToHost()
{
    //Check the socket pointer.
    if(socket()==nullptr)
    {
        //Set the configuration first.
        return false;
    }
    //Get the pop socket.
    QTcpSocket *popSocket=socket();
    const KNMailProtocolConfig &config=account()->receiveConfig();
    //Check the connection type, it will use different connect commands.
    switch(config.socketType)
    {
    case SocketTcp:
    case SocketTls:
        //Connection is simple, use plain type connection.
        popSocket->connectToHost(config.hostName, config.port);
        break;
    case SocketSsl:
    {
        //Recast the socket as SSL socket,
        QSslSocket *sslSocket=static_cast<QSslSocket *>(popSocket);
        sslSocket->setProtocol(QSsl::TlsV1_0OrLater);
        sslSocket->connectToHostEncrypted(config.hostName, config.port);
        break;
    }
    }
    //Wait for the socket response.
    if(!popSocket->waitForConnected(connectionTimeout()))
    {
        //Set the last error to be connection time out.
        setLastError(ConnectionTimeout);
        //Fail to get response.
        return false;
    }
    //Wait for response from server.
    if(!popSocket->waitForReadyRead(responseTimeout()))
    {
        //Failed to read response.
        setLastError(ResponseTimeout);
        //Mission complete.
        return false;
    }
    //Read all lines.
    QString lineCache;
    while(popSocket->canReadLine())
    {
        //Read the line.
        lineCache=popSocket->readLine();
        //Check out the repsonse text.
        if(lineCache.startsWith("+OK"))
        {
            //Connection success.
            return true;
        }
    }
    //No line mentioned +OK, means error happens.
    return false;
}

bool KNMailPop3Protocol::login()
{
    //Get the socket.
    QTcpSocket *popSocket=socket();
    //Check the state of pop socket.
    if(popSocket->state()!=QAbstractSocket::ConnectedState &&
            //Socket is not online, try to re-connect.
            (!connectToHost()))
    {
        //Failed to connect to host, then failed to login.
        return false;
    }
    //Get the account.
    KNMailAccount *mailAccount=account();
    //Reset the last error.
    setLastError(-1);
    //! FIXME: Replace this with a better authentication.
    //Send USER message.
    if(!sendMessage("USER " + mailAccount->username()))
    {
        //Send message error.
        return false;
    }
    if(!waitAndCheckResponse())
    {
        //Check the last error, if connection is okay,
        if(lastError()==-1)
        {
            //it must be user name error.
            setLastError(LoginUsernameError);
        }
        return false;
    }
    //Send PASS message.
    if(!sendMessage("PASS " + mailAccount->password()))
    {
        return false;
    }
    //Wait for response.
    if(!waitAndCheckResponse())
    {
        //Check the last error, if connection is okay,
        if(lastError()==-1)
        {
            //it must be password error.
            setLastError(LoginPasswordError);
        }
        return false;
    }
    //Now we should login the server with the specific username and password.
    return true;
}

inline bool KNMailPop3Protocol::waitAndCheckResponse()
{
    //We have to try 3 times.
    int tries=5;
    while(tries--)
    {
        //Wait for response.
        QString popResponse;
        if(waitForResponse(&popResponse))
        {
            //Check the response.
            //We got OK from the server, that means mission complete.
            if(popResponse.startsWith("+OK"))
            {
                return true;
            }
            //Or else, login data error.
            return false;
        }
    }
    //We have tried it for several times, but it cannot get the data.
    return false;
}
