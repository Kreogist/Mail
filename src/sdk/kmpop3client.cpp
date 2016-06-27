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

//#include "mime/kmmailbody.h"

#include "kmpop3client.h"

KMPop3Client::KMPop3Client(QObject *parent) :
    KMAbstractReceiverClient(parent)
{
    //Set the default port to be 110.
    setPort(110);
}

KMPop3Client::KMPop3Client(const QString &host,
                           quint16 port,
                           KMAbstractMailClient::ConnectionType ct,
                           QObject *parent) :
    KMAbstractReceiverClient(parent)
{
    //Set the host url.
    setHost(host);
    //Set the port to be user configuration.
    setPort(port);
    //Set the connection type to rebuild the socket.
    setConnectionType(ct);
}

bool KMPop3Client::connectToHost()
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
            if(responseText.startsWith("+OK"))
            {
                //The message we want.
                return true;
            }
        }
    }
    return false;
}

bool KMPop3Client::login()
{
    //Check socket state.
    if((!isConnected()) &&
            (!connectToHost()))
    {
        //Cannot connect to host server, failed to login.
        return false;
    }
    //In POP3, we will ignore the auth settings.
    //Send user name.
    sendMessage("USER " + QByteArray().append(userName()));
    //Wait for server response and check out the response.
    if(!waitAndCheckResponse())
    {
        //Server doesn't response, or the server is not working.
        return false;
    }
    //Send the password.
    sendMessage("PASS " + QByteArray().append(password()));
    //Wait for server response and check out the response.
    return waitAndCheckResponse();
}

bool KMPop3Client::quit()
{
    //Check whether the server is connected.
    if(!isConnected())
    {
        //If we didn't connect to server,
        return true;
    }
    //Send quit message.
    sendMessage("QUIT");
    //Wait for sever response.
    return waitAndCheckResponse();
}

void KMPop3Client::getMailList()
{
    //Check connecting state.
    if((!isConnected()) && (!login()))
    {
        //If we cannot login, then failed to get mail list.
        return;
    }
    //Send STAT command to get result of the mail list.
    sendMessage("STAT");
    //Wait and get the result.
    if(!waitForResponse() || (responseText().size()<3) ||
            (!responseText().startsWith("+OK")))
    {
        //Timeout, we won't get any message.
        qDebug()<<"Server Timeout. Cannot get STAT result.";
        return;
    }
    //Get the mail count.
    //The format of result text is
    //  +OK 1625 740534731
    //Means 1625 mails, total 740534731 bytes.
    QString &&resultText=responseText().mid(3).simplified();
    //Check out the result.
    int firstSpace=resultText.indexOf(' ');
    //Check the first position of the space.
    if(firstSpace<0)
    {
        qDebug()<<"STAT return a strange result.";
        return;
    }
    //Get the mail count.
    qlonglong mailCount=resultText.left(firstSpace).toLongLong();
    //Receive the mail from server.
//    mailCount=qMin(10, mailCount);
//    while(mailCount--)
//    {
        //Use RETR to get message from server.
//        sendMessage("RETR "+QString::number(mailCount));
    qDebug()<<"Mail Count:"<<mailCount;
    sendMessage("RETR "+QString::number(mailCount));
    QByteArray lineData, totalData;
    while(true)
    {
        if(!socket()->waitForReadyRead())
        {
            qDebug()<<"Time out?!";
            break;
        }
        else
        {
            bool firstLine=true;
            while(socket()->canReadLine())
            {
                //Read mail line data.
                lineData=socket()->readLine();
                //Check the header of line data.
                if(firstLine && lineData.startsWith("+OK"))
                {
                    //Set first line flag.
                    firstLine=false;
                    //Read next line.
                    continue;
                }
                //Append to total data.
                totalData.append(lineData);
            }
        }
        //Check line data.
        if(lineData==".\r\n")
        {
            break;
        }
    }
    qDebug()<<totalData;
//    }
}

void KMPop3Client::updateResponseCode(const QString &responseText)
{
    //Nothing to do with response code.
    Q_UNUSED(responseText);
}

inline bool KMPop3Client::waitAndCheckResponse()
{
    //Wait for server response and check out the response.
    return (waitForResponse()) && (responseText().startsWith("+OK"));
}

