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
#include <QEventLoop>
#include <QTimer>

#include "knmailaccount.h"

#include "knmailprotocol.h"

KNMailProtocol::KNMailProtocol(QObject *parent) :
    QObject(parent),
    m_socket(nullptr),
    m_account(nullptr),
    m_lastError(-1),
    m_connectionTimeout(5000),
    m_responseTimeout(5000),
    m_sendTimeout(30000),
    m_dataWritten(false)
{
}

KNMailProtocol::~KNMailProtocol()
{
    //Recover socket memory.
    if(m_socket)
    {
        delete m_socket;
    }
}

QTcpSocket *KNMailProtocol::socket() const
{
    return m_socket;
}

void KNMailProtocol::setLastError(int errorCode)
{
    qDebug()<<"Error changed to"<<errorCode;
    m_lastError=errorCode;
}

void KNMailProtocol::setSocketType(MailProtocolSocket type)
{
    //Update the socket.
    if(m_socket)
    {
        //Disconnect all connection related to socket.
        disconnect(m_socket, 0, 0, 0);
        //Delete the socket.
        delete m_socket;
        //Reset socket pointer.
        m_socket=nullptr;
    }
    //Construct the communication socket.
    switch(type)
    {
    case SocketTcp:
        //Plain text connection, use TCP socket.
        m_socket=new QTcpSocket();
        break;
    case SocketTls:
    case SocketSsl:
        //Encryption connection, use SSL socket.
        m_socket=new QSslSocket();
        break;
    }
#ifdef Q_OS_WIN
    //Link the socket for Windows platform.
    connect(m_socket, &QTcpSocket::bytesWritten,
            [=](const qint64 &bytes)
    {qDebug()<<"Send"<<bytes<<"bytes.";m_dataWritten=true;m_waitWriteLoop.quit();});
    connect(m_socket, &QTcpSocket::readyRead,
            &m_waitReadLoop, &QEventLoop::quit);
#endif
}

bool KNMailProtocol::sendMessage(const QString &message)
{
    //Check socket first.
    if(m_socket==nullptr)
    {
        qDebug()<<"Send Message Error: no socket error.";
        //No socket, no send message.
        return false;
    }
    qDebug()<<"Send Message"<<message.toLatin1()+"\r\n";
#ifdef Q_OS_WIN
    //Reset the flag.
    m_dataWritten=false;
#endif
    //Write the data to socket.
    m_socket->write(message.toLatin1() + "\r\n");
    //Clear the cache.
    m_socket->flush();
    //Wait for socket write all the data.
#ifdef Q_OS_WIN
    QTimer::singleShot(m_responseTimeout, &m_waitWriteLoop, &QEventLoop::quit);
    m_waitWriteLoop.exec();
    if(m_dataWritten)
    {
        return true;
    }
#else
    if(m_socket->waitForBytesWritten(m_sendTimeout))
    {
        qDebug()<<"Send"<< message.size() <<"bytes.";
        //All the data has been written.
        return true;
    }
#endif
    qDebug()<<"Error!";
    //Send data time out error.
    setLastError(SendDataTimeout);
    //Failed to write data.
    return false;
}

bool KNMailProtocol::waitForResponse(QString *responseText)
{
#ifdef Q_OS_WIN
    //Check the socket state.
    if(!m_socket->canReadLine())
    {
        //Timeout timer.
        QTimer::singleShot(m_responseTimeout,
                           &m_waitReadLoop, &QEventLoop::quit);
        //Start to execute.
        m_waitReadLoop.exec();
    }
#else
    //Wait for the client read read.
    if(!m_socket->waitForReadyRead(m_responseTimeout))
    {
        //Save the last error.
        setLastError(ResponseTimeout);
        //Fail to wait for ready read.
        return false;
    }
#endif
    qDebug()<<"Start reading response.";
    //Read until there's no line to read.
    while(m_socket->canReadLine())
    {
        //Save the server response.
        QString lineCache=m_socket->readLine();
        qDebug()<<lineCache;
        //Check the response text pointer.
        if(responseText)
        {
            //Append data to the cache.
            responseText->append(lineCache);
        }
    }
    //Mission complete.
    return true;
}

KNMailAccount *KNMailProtocol::account() const
{
    return m_account;
}

void KNMailProtocol::setAccount(KNMailAccount *account)
{
    //Save the account pointer.
    m_account = account;
    //Update the configuration.
    updateProtocolConfig();
}

void KNMailProtocol::setResponseTimeout(int responseTimeout)
{
    m_responseTimeout = responseTimeout;
}

void KNMailProtocol::setConnectionTimeout(int connectionTimeout)
{
    m_connectionTimeout = connectionTimeout;
}
