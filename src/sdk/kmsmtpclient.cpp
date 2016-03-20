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

#include "mime/kmmailbody.h"

#include "kmsmtpclient.h"

#include <QDebug>

KMSmtpClient::KMSmtpClient(QObject *parent) :
    KMAbstractSenderClient(parent)
{
    //Set the default port to be 25.
    setPort(25);
}

KMSmtpClient::KMSmtpClient(const QString &host,
                           quint16 port,
                           KMSmtpClient::ConnectionType ct,
                           QObject *parent) :
    KMAbstractSenderClient(parent)
{
    //Set the host url.
    setHost(host);
    //Set the port to be user configuration.
    setPort(port);
    //Set the connection type to rebuild the socket.
    setConnectionType(ct);
}

bool KMSmtpClient::connectToHost()
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
        qDebug()<<"Start normal link, host:"<<host()<<"port:"<<port();
        //Simply call the connect to host function.
        socket()->connectToHost(host(), port());
        break;
    case SslConnection:
        qDebug()<<"Start SSL link, host:"<<host()<<"port:"<<port();
        //Recast socket as a QSslSocket.
        static_cast<QSslSocket *>(socket())->connectToHostEncrypted(host(),
                                                                    port());
        break;
    }
    qDebug()<<"Start to connect.";
    //Tries to connect to server
    if(!socket()->waitForConnected(connectionTimeout()))
    {
        //Emit the error.
        emit clientError(ConnectionTimeoutError);
        //Failed to connect.
        return false;
    }
    // If the response code is not 220 (Service ready)
    // means that is something wrong with the server
    //The response code needs to be 220.
    if(!waitAndCheckResponse(220, ServerError))
    {
        //Failed to login.
        return false;
    }
    qDebug()<<"Start EHLO";
    // Send a EHLO/HELO message to the server
    // The client's first command must be EHLO/HELO
    sendMessage("EHLO " + userName());
    //The response code needs to be 250.
    if(!waitAndCheckResponse(250, ServerError))
    {
        //Failed to login.
        return false;
    }
    //If the connection type is TLS connection, we have to start TLS.
    if(connectionType() == TlsConnection)
    {
        //Send a request to start TLS handshake.
        sendMessage("STARTTLS");
        //The response code needs to be 220.
        if(!waitAndCheckResponse(220, ServerError))
        {
            //Failed to login.
            return false;
        }
        //Recast the socket into ssl socket.
        QSslSocket *sslSocket=static_cast<QSslSocket *>(socket());
        //Start encryption.
        qDebug()<<"Start encryption.";
        sslSocket->startClientEncryption();
        sslSocket->ignoreSslErrors();
        //Check out result.
        if(!sslSocket->waitForEncrypted(connectionTimeout()))
        {
            //Print out the error information.
            qCritical()<<sslSocket->errorString();
            //Emit the smtp error.
            emit clientError(ConnectionTimeoutError);
            //Failed to connect.
            return false;
        }
        qDebug()<<"Start EHLO again";
        // Send ELHO one more time
        sendMessage("EHLO " + userName());
        //The response code needs to be 250.
        if(!waitAndCheckResponse(250, ServerError))
        {
            //Failed to login.
            return false;
        }
    }
    //Mission complete.
    return true;
}

bool KMSmtpClient::login()
{
    //Check out state.
    if((!isConnected()) && (!connectToHost()))
    {
        //Means socket is still not connected to server or already login.
        return false;
    }
    //Check auth method.
    if(authMethod()==AuthPlain)
    {
        //Sending command: AUTH PLAIN base64('\0' + username + '\0' + password)
        sendMessage("AUTH PLAIN " + QByteArray().append((char)0x00)
                    .append(userName()).append((char)0x00)
                    .append(password()).toBase64());
        // The response code needs to be 235.
        if(!waitAndCheckResponse(235, ServerError))
        {
            //Failed to login.
            return false;
        }
        //Mission complete.
        return true;
    }
    //Then the method should be auth login.
    // Sending command: AUTH LOGIN
    sendMessage("AUTH LOGIN");
    //The response code needs to be 334.
    if(!waitAndCheckResponse(334, AuthenticationFailedError))
    {
        //Failed to login.
        return false;
    }
    // Send the username in base64
    sendMessage(QByteArray().append(userName()).toBase64());
    //The response code needs to be 334.
    if(!waitAndCheckResponse(334, AuthenticationFailedError))
    {
        //Failed to login.
        return false;
    }
    // Send the password in base64
    sendMessage(QByteArray().append(password()).toBase64());
    //If the response is not 235 then the authentication was faild
    if(!waitAndCheckResponse(235, AuthenticationFailedError))
    {
        //Failed to login.
        return false;
    }
    //Mission compelte.
    return true;
}

bool KMSmtpClient::quit()
{
    //Check out the state.
    if(!isConnected())
    {
        //Don't need quit.
        return true;
    }
    //Send quit command.
    sendMessage("QUIT");
    //Wait for response.
    //If the response is not 221 then someting wrong with mail server.
    if(!waitAndCheckResponse(221, ServerError))
    {
        //Failed to login.
        return false;
    }
    //Get Bye from server mission complete.
    return true;
}

void KMSmtpClient::updateResponseCode(const QString &responseText)
{
    //Extract the respose code from the server's responce
    //(first 3 digits)
    setResponseCode(responseText.left(3).toInt());
}

void KMSmtpClient::sendMail(KMMailBody *mail)
{
    //Check mail pointer.
    if(mail==nullptr || ((!isConnected()) && (!login())))
    {
        //Mail is null, ignore the request.
        //Cannot connected to host.
        return;
    }
    //Send the mail message.
    //Send the MAIL command with the sender.
    sendMessage("MAIL FROM: <" + mail->element(KMMailBody::ElementSender) +
                ">");
    //If the response is not 250 then someting wrong with mail server.
    if(!waitAndCheckResponse(250, ServerError))
    {
        //Failed to send message.
        return;
    }
    //Prepare the contact name and address container.
    QStringList contactName, contactAddress;
    //Send RCPT command for each recipient.
    // To (primary recipients)
    mail->contactInfo(KMMailBody::Receiver, contactAddress, contactName);
    for(auto i=contactAddress.begin(); i!=contactAddress.end(); ++i)
    {
        //Send RCPT TO message.
        sendMessage("RCPT TO: <" + (*i) + ">");
        //If the response is not 250 then someting wrong with mail server.
        if(!waitAndCheckResponse(250, ServerError))
        {
            //Failed to send message.
            return;
        }
    }
    // Cc (carbon copy)
    mail->contactInfo(KMMailBody::CarbonCopy, contactAddress, contactName);
    for(auto i=contactAddress.begin(); i!=contactAddress.end(); ++i)
    {
        //Send RCPT TO message.
        sendMessage("RCPT TO: <" + (*i) + ">");
        //If the response is not 250 then someting wrong with mail server.
        if(!waitAndCheckResponse(250, ServerError))
        {
            //Failed to send message.
            return;
        }
    }
    // Bcc (blind carbon copy)
    mail->contactInfo(KMMailBody::BlindCarbonCopy, contactAddress, contactName);
    for(auto i=contactAddress.begin(); i!=contactAddress.end(); ++i)
    {
        //Send RCPT TO message.
        sendMessage("RCPT TO: <" + (*i) + ">");
        //If the response is not 250 then someting wrong with mail server.
        if(!waitAndCheckResponse(250, ServerError))
        {
            //Failed to send message.
            return;
        }
    }

    //Send DATA command.
    sendMessage("DATA");
    //If the response is not 354 then someting wrong with mail server.
    if(!waitAndCheckResponse(354, ServerError))
    {
        //Failed to send message.
        return;
    }
    //Send the E-mail data.
    sendMessage(mail->toString());
    // Send \r\n.\r\n to end the mail data
    sendMessage(".");
    //Check response.
    waitAndCheckResponse(250, ServerError);
    //Misson complete.
    return;
}

inline bool KMSmtpClient::waitAndCheckResponse(
        int captureCode,
        KMAbstractMailClient::MailClientError failedError)
{
    //Wait for server response.
    if(!waitForResponse())
    {
        //Server doesn't response.
        return false;
    }
    qDebug()<<"Response code is:"<<responseCode()<<"Capture code is:"<<captureCode;
    // The response code needs to be 250.
    if(responseCode() != captureCode)
    {
        //Server error.
        emit clientError(failedError);
        //Failed to connect.
        return false;
    }
    //Connect and response correct.
    return true;
}
