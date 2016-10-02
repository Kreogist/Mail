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

#include "knmailsmtpprotocol.h"

#include <QDebug>

KNMailSmtpProtocol::KNMailSmtpProtocol(QObject *parent) :
    KNMailSendProtocol(parent)
{
}

bool KNMailSmtpProtocol::connectToHost()
{
    //Get the receive config.
    const KNMailProtocolConfig &config=account()->sendConfig();
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
    return waitAndCheckResponse(200);
}

bool KNMailSmtpProtocol::login()
{
    return false;
}

bool KNMailSmtpProtocol::waitAndCheckResponse(int captureCode)
{
    QByteArray responseText;
    //Wait for server response.
    if(!waitForResponse(&responseText))
    {
        //Server doesn't response.
        return false;
    }
    qDebug()<<responseText;
    return true;
}
