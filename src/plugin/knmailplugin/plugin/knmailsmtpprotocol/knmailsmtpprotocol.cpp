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
#include <QTcpSocket>

#include "mime/knmimepart.h"
#include "knmailaccount.h"
#include "knmailsmtpprotocol.h"

#include <QDebug>

KNMailSmtpProtocol::KNMailSmtpProtocol(QObject *parent) :
    KNMailSenderProtocol(parent),
    m_loginState(false)
{
}

bool KNMailSmtpProtocol::connectToHost()
{
    qDebug()<<"Start to connect.";
    //After connection, the login state reset to false.
    m_loginState=false;
    //Get the receive config.
    const KNMailProtocolConfig &config=account()->sendConfig();
    //Connect to host server.
    if(!startConnection(config))
    {
        //Can't connect to server.
        setLastError(ConnectionFailed);
        qDebug()<<"Connected failed.";
        return false;
    }
    qDebug()<<"Wait for connected response.";
    //Wait for server response.
    if(!waitForConnection())
    {
        //Set the last error to be connection timeout.
        setLastError(ConnectionTimeout);
        //Failed to get response.
        return false;
    }
    //Check the response.
    if(!waitAndCheckResponse(220))
    {
        //Failed to connect with server.
        return false;
    }
    //Send EHLO.
    sendMessage("EHLO localhost");
    //Wait and check response.
    if(!waitAndCheckResponse(250))
    {
        //Failed.
        return false;
    }
    //Check tls.
    if(account()->sendConfig().socketType==SocketTls)
    {
        //Start TLS command.
        sendMessage("STARTTLS");
        //Check response.
        if(!waitAndCheckResponse(220))
        {
            return false;
        }
        //Do another ELHO.
        sendMessage("EHLO localhost");
        //Wait for response.
        waitForResponse();
    }
    return true;
}

bool KNMailSmtpProtocol::login()
{
    //Start to login.
    if(m_loginState)
    {
        //No need to login.
        return true;
    }
    //Check the state of pop socket.
    if(socket() && socket()->state()!=QAbstractSocket::ConnectedState &&
            //Socket is not online, try to re-connect.
            (!connectToHost()))
    {
        //Failed to connect to host, then failed to login.
        return false;
    }
    //Start to login.
    sendMessage("AUTH LOGIN");
    //Check the response.
    if(!waitAndCheckResponse(334))
    {
        //Failed to login.
        return false;
    }
    //Type the username.
    sendMessage(account()->username().toLatin1().toBase64());
    if(!waitAndCheckResponse(334))
    {
        //Failed to login.
        return false;
    }
    //Type the password.
    sendMessage(account()->password().toLatin1().toBase64());
    if(!waitAndCheckResponse(235))
    {
        //Failed to login.
        return false;
    }
    m_loginState=true;
    return true;
}

bool KNMailSmtpProtocol::sendMail(const QString &receiverAddress,
                                  KNMimePart *mimePart)
{
    //Check login state first.
    if(m_loginState==false && !login())
    {
        //Failed to login.
        return false;
    }
    //Prepare the list.
    QStringList mimeDataList;
    {
        //Translate the MIME part to string.
        QString mimeData=mimePart->toString();
        //Split the data.
        mimeDataList=mimeData.split("\r\n");
    }
    qDebug()<<mimeDataList;
    //First send MAIL FROM:
    sendMessage(QString("MAIL FROM: <%1>").arg(account()->username()));
    //Wait for response.
    if(!waitAndCheckResponse(250))
    {
        //Failed to send.
        return false;
    }
    //Second, send the receiver.
    sendMessage(QString("RCPT TO: <%1>").arg(receiverAddress));
    //Wait for response.
    if(!waitAndCheckResponse(250))
    {
        //Failed to send.
        return false;
    }
    //Third, send data.
    sendMessage("DATA");
    //Wait for response.
    if(!waitAndCheckResponse(354))
    {
        //Failed to send.
        return false;
    }
    //Send all the content.
    for(int i=0; i<mimeDataList.size(); ++i)
    {
        //Send one message.
        if(!sendMessage(mimeDataList.at(i)))
        {
            //Failed to write content.
            return false;
        }
    }
    //Write "."
    sendMessage(".");
    //Check for result.
    return waitAndCheckResponse(250);
}

bool KNMailSmtpProtocol::logout()
{
    //Use quit.
    sendMessage("quit");
    //Check the response.
    if(waitAndCheckResponse(221))
    {
        //Failed to logout.
        return false;
    }
    //Change the login state.
    m_loginState=false;
    return true;
}

bool KNMailSmtpProtocol::waitAndCheckResponse(int targetNumber)
{
    //Cache for result.
    QList<QByteArray> responseResult;
    //Wait for response.
    if(waitForResponse(&responseResult))
    {
        //Get the response data.
        QByteArray lastLine=responseResult.last();
        //Find first space.
        int numberBounder=lastLine.indexOf(' ');
        //Get the number
        numberBounder=lastLine.left(numberBounder).toInt();
        //Check the number.
        return numberBounder==targetNumber;
    }
    return false;
}
