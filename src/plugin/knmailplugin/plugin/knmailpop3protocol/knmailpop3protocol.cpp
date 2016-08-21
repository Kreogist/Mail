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
    KNMailReceiverProtocol(parent)
{
}

bool KNMailPop3Protocol::connectToHost()
{
    //Get the receive config.
    const KNMailProtocolConfig &config=account()->receiveConfig();
    //Connect to host server.
    if(!startConnection(config))
    {
        //Can't connect to server.
        setLastError(ConnectionFailed);
        return false;
    }
    //Wait for the socket response.
    if(!waitForConnection())
    {
        //Set the last error to be connection time out.
        setLastError(ConnectionTimeout);
        //Fail to get response.
        return false;
    }
    //Wait for response from server.
    return waitAndCheckResponse();
}

bool KNMailPop3Protocol::login()
{
    //Check the state of pop socket.
    if(socket()->state()!=QAbstractSocket::ConnectedState &&
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
    //We have to try several times.
    int tries=5;
    //Do tries times loop.
    while(tries--)
    {
        //Wait for response.
        QByteArray popResponse;
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
