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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTcpSocket>
#include <QSslSocket>

#include "kmimapclient.h"

KMImapClient::KMImapClient(QObject *parent) :
    KMAbstractReceiverClient(parent),
    m_counter(0)
{
    //Set port to be 143 as default.
    setPort(143);
}

KMImapClient::KMImapClient(const QString &host,
                           quint16 port,
                           KMAbstractMailClient::ConnectionType ct,
                           QObject *parent) :
    KMAbstractReceiverClient(parent),
    m_counter(0)
{
    //Set the host url.
    setHost(host);
    //Set the port to be user configuration.
    setPort(port);
    //Set the connection type to rebuild the socket.
    setConnectionType(ct);
}

bool KMImapClient::connectToHost()
{
    //Check out the socket pointer.
    if(socket()==nullptr)
    {
        qCritical()<<"Socket pointer is NULL.";
        return false;
    }
    //Check out the connection type.
    switch (connectionType())
    {
    case TlsConnection:
    case TcpConnection:
        //Simply call the connect to host function.
        socket()->connectToHost(host(), port());
        break;
    case SslConnection:
        //Recast socket as a QSslSocket.
        static_cast<QSslSocket *>(socket())->connectToHostEncrypted(host(),
                                                                    port());
        break;
    }
    //Tries to connect to server
    if(!socket()->waitForConnected(connectionTimeout()))
    {
        //Emit the error.
        emit clientError(ConnectionTimeoutError);
        //Failed to connect.
        return false;
    }
    //Wait for server response and check out the response.
    while(true)
    {
        //Wait for the client ready read.
        if(!socket()->waitForReadyRead(responseTimeout()))
        {
            qDebug()<<"Time out.";
            //Emit the time out error.
            emit clientError(ResponseTimeoutError);
            //Failed to get response.
            return false;
        }
        QString responseText;
        //Read all lines.
        while(socket()->canReadLine())
        {
            //Save the server response.
            responseText=socket()->readLine();
            qDebug()<<responseText;
            //Check out the response text.
            if(responseText.startsWith("* OK"))
            {
                //Reset the counter.
                m_counter=0;
                //The message we want.
                return true;
            }
        }
    }
    return false;
}

bool KMImapClient::login()
{
    //In IMAP, we seems need to ignore the auth settings.
    //Send message to server.
    sendImapMessage("login "+userName()+" \""+password()+"\"");
    //Wait for server response.
    if(!waitAndCheckResponse())
    {
        //Failed to login.
        return false;
    }
    //For some IMAP Client, like Netease, we need to use IMAP ID command to
    //register our product.
    sendImapMessage("ID (\"name\" \"KreogistMail\" "
                        "\"version\" \"1.0\" "
                        "\"vendor\" \"Kreogist\")");
    //Wait for response, e.g. A002 OK ID completed
    return waitAndCheckResponse();
}

bool KMImapClient::quit()
{
    //Send logout to quit.
    sendImapMessage("logout");
    //Wait for response. e.g. A004 OK LOGOUT completed
    return waitAndCheckResponse();
}

void KMImapClient::getMailList()
{
    ;
}

void KMImapClient::updateResponseCode(const QString &responseText)
{
    //There's no response code in IMAP protocol.
    Q_UNUSED(responseText)
}

bool KMImapClient::messageFilter()
{
    //Find the first space.
    int firstSpacePosition=responseText().indexOf(' ');
    //Check the space position, if the position is not correct.
    return firstSpacePosition==(qMax(QString::number(m_counter).size(), 3)+1);
}

inline bool KMImapClient::waitAndCheckResponse()
{
    //Wait for server response.
    if(waitForResponse())
    {
        //The message should be kind of:
        //  A001 OK LOGIN completed\r\n
        //Find the OK after A001.
        //Find the first space.
        int firstSpace=responseText().indexOf(' ');
        //Check whether the first space exist.
        if(firstSpace==-1)
        {
            //Failed to find OK.
            return false;
        }
        //Check the string size.
        return (responseText().size()>=firstSpace+2) &&
                (responseText().mid(firstSpace+1, 2)=="OK");
    }
    //Cannot get response from server.
    return false;
}

inline bool KMImapClient::sendImapMessage(const QString &command)
{
    //Send the full command to server.
    return sendMessage(QString('A') +
                       QString::number(++m_counter).rightJustified(3, '0') +
                       QString(' ') + command);
}

