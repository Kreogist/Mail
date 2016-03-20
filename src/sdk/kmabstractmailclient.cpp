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

#include "kmabstractmailclient.h"

#include <QDebug>

KMAbstractMailClient::KMAbstractMailClient(QObject *parent) :
    QObject(parent),
    m_host(QString("localhost")),
    m_user(QString("localhost")),
    m_password(QString()),
    m_responseText(QString()),
    m_port(0),
    m_socket(nullptr),
    m_connectionType(TcpConnection),
    m_authMethod(AuthPlain),
    m_connectionTimeout(5000),
    m_responseTimeout(5000),
    m_sendMessageTimeout(60000),
    m_responseCode(-1)
{
    //Set the connection type to rebuild the socket.
    setConnectionType(TcpConnection);
}

KMAbstractMailClient::~KMAbstractMailClient()
{
    //Recycle socket object.
    if(m_socket)
    {
        delete m_socket;
    }
}

quint16 KMAbstractMailClient::port() const
{
    return m_port;
}

void KMAbstractMailClient::setPort(quint16 port)
{
    m_port = port;
}

KMAbstractMailClient::AuthMethod KMAbstractMailClient::authMethod() const
{
    return m_authMethod;
}

void KMAbstractMailClient::setAuthMethod(const AuthMethod &authMethod)
{
    m_authMethod = authMethod;
}

void KMAbstractMailClient::setPassword(const QString &password)
{
    m_password = password;
}

QString KMAbstractMailClient::userName() const
{
    return m_user;
}

void KMAbstractMailClient::setUserName(const QString &user)
{
    m_user = user;
}

QString KMAbstractMailClient::host() const
{
    return m_host;
}

void KMAbstractMailClient::setHost(const QString &host)
{
    m_host = host;
}

int KMAbstractMailClient::responseCode() const
{
    return m_responseCode;
}

KMAbstractMailClient::ConnectionType
KMAbstractMailClient::connectionType() const
{
    return m_connectionType;
}

void KMAbstractMailClient::setConnectionType(
        const ConnectionType &connectionType)
{
    //Save the connection type.
    m_connectionType = connectionType;
    //Check socket pointer.
    if(m_socket!=nullptr)
    {
        //Delete socket object.
        delete m_socket;
        //Reset socket pointer.
        m_socket=nullptr;
    }
    //Rebuild socket pointer.
    switch(m_connectionType)
    {
    case TcpConnection:
        m_socket=new QTcpSocket;
        break;
    case SslConnection:
    case TlsConnection:
        m_socket=new QSslSocket;
        break;
    }
}

int KMAbstractMailClient::sendMessageTimeout() const
{
    return m_sendMessageTimeout;
}

void KMAbstractMailClient::setSendMessageTimeout(int msec)
{
    m_sendMessageTimeout = msec;
}

int KMAbstractMailClient::responseTimeout() const
{
    return m_responseTimeout;
}

void KMAbstractMailClient::setResponseTimeout(int msec)
{
    m_responseTimeout = msec;
}

bool KMAbstractMailClient::waitForResponse()
{
    while(true)
    {
        //Wait for the client ready read.
        if(!m_socket->waitForReadyRead(m_responseTimeout))
        {
            qDebug()<<"Time out.";
            //Emit the time out error.
            emit clientError(ResponseTimeoutError);
            //Failed to get response.
            return false;
        }

        while(m_socket->canReadLine())
        {
            //Save the server response.
            m_responseText=m_socket->readLine();
            //Update the response code according to the text.
            updateResponseCode(m_responseText);
            //Check out the response code.
            if(m_responseCode/100 == 4)
            {
                //Server error catched.
                emit clientError(ServerError);
            }
            if(m_responseCode/100 == 5)
            {
                //Client error catched.
                emit clientError(ClientError);
            }
            qDebug()<<m_responseText;
            //Check out the response text.
            if(messageFilter())
            {
                //The message we want.
                return true;
            }
        }
    }
}

bool KMAbstractMailClient::sendMessage(const QString &text)
{
    qDebug()<<"Send message:"<<text;
    //Write the text to the socket.
    m_socket->write(text.toUtf8() + "\r\n");
    m_socket->flush();
    //Wait for socket.
    if(!m_socket->waitForBytesWritten(m_sendMessageTimeout))
    {
        //Send data time out.
        emit clientError(SendDataTimeoutError);
        //Failed to send message.
        return false;
    }
    //Success.
    return true;
}

QTcpSocket *KMAbstractMailClient::socket() const
{
    return m_socket;
}

QString KMAbstractMailClient::password() const
{
    return m_password;
}

void KMAbstractMailClient::setResponseCode(int responseCode)
{
    m_responseCode = responseCode;
}

QString KMAbstractMailClient::responseText() const
{
    return m_responseText;
}

bool KMAbstractMailClient::messageFilter()
{
    //For SMTP, the message will be return as the following:
    // 250 8BITMIME
    //The first 3 bytes is the state value, this will be used as default value.
    return m_responseText[3]==' ';
}

bool KMAbstractMailClient::isConnected() const
{
    return (m_socket!=nullptr) &&
            (m_socket->state()==QAbstractSocket::ConnectedState);
}

int KMAbstractMailClient::connectionTimeout() const
{
    return m_connectionTimeout;
}

void KMAbstractMailClient::setConnectionTimeout(int msec)
{
    m_connectionTimeout = msec;
}
