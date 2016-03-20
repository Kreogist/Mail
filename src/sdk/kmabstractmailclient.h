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

#ifndef KMABSTRACTMAILCLIENT_H
#define KMABSTRACTMAILCLIENT_H

#include <QObject>

class QTcpSocket;
/*!
 * \brief The KMAbstractMailClient class provide the basic functions of a mail
 * client procotol will be used. This abstract client provides the basic methods
 * like waiting for response, connect to host.
 */
class KMAbstractMailClient : public QObject
{
    Q_OBJECT
public:
    enum AuthMethod
    {
        AuthPlain,
        AuthLogin
    };

    enum MailClientError
    {
        ConnectionTimeoutError,
        ResponseTimeoutError,
        SendDataTimeoutError,
        AuthenticationFailedError,
        ServerError,    // 4xx smtp error
        ClientError     // 5xx smtp error
    };

    enum ConnectionType
    {
        TcpConnection,
        SslConnection,
        TlsConnection
    };

    /*!
     * \brief Constrcut a KMAbstractMailClient object.
     * \param parent The parent object.
     */
    explicit KMAbstractMailClient(QObject *parent = 0);
    ~KMAbstractMailClient();

    /*!
     * \brief Get the host url of protocol client.
     * \return The host url. The default value of this property is localhost.
     */
    QString host() const;

    /*!
     * \brief Set the host server url.
     * \param host The client server host url.
     */
    void setHost(const QString &host);

    /*!
     * \brief Get the user name.
     * \return User name which will be used when login. The default value is a
     * null string.
     */
    QString userName() const;

    /*!
     * \brief Set the login user name.
     * \param user Login user name.
     */
    void setUserName(const QString &userName);

    /*!
     * \brief Set the login used password, this is a write only property. Outter
     * object can only set the password but never read it again.
     * \param password The login password. The default value is a null string.
     */
    void setPassword(const QString &password);

    /*!
     * \brief Set the authentication method.
     * \return Current using authentication method. The default value is
     * AuthPlain.
     */
    AuthMethod authMethod() const;

    /*!
     * \brief Set the authentication method.
     * \param authMethod Login using authentication method.
     */
    void setAuthMethod(const AuthMethod &authMethod);

    /*!
     * \brief Get the port number of protocol client will be used to
     * communication.
     * \return Number of the protocol port. The default value is 0.
     */
    quint16 port() const;

    /*!
     * \brief Set the port number of protocol client.
     * \param port The client used port number.
     */
    void setPort(quint16 port);

    /*!
     * \brief Get the current connection type.
     * \return The connection type. The default value is TcpConnection.
     */
    ConnectionType connectionType() const;

    /*!
     * \brief Set the protocol communication connection type.
     * \param connectionType New connection type.
     */
    void setConnectionType(const ConnectionType &connectionType);

    /*!
     * \brief Get the response timeout bound. Unit is msecond.
     * \return The response timeout bound. Default value is 5000 ms.
     */
    int responseTimeout() const;

    /*!
     * \brief Set the new response timeout bound.
     * \param msec Time out bound. The Unit is ms.
     */
    void setResponseTimeout(int msec);

    /*!
     * \brief Get the message sending timeout bound. Unit is msecond.
     * \return The message sending timeout bound. Default value is 60000 ms. (1
     * minate)
     */
    int sendMessageTimeout() const;

    /*!
     * \brief Set the message send timeout. Unit is msecond.
     * \param msec Time out bound. The unit is ms.
     */
    void setSendMessageTimeout(int msec);

    /*!
     * \brief Get the response code of the protocol result.
     * \return The response code from protocol server. This code is invalid
     * before any operations is done.
     */
    int responseCode() const;

    /*!
     * \brief Get the connection timeout bound. Unit is msecond.
     * \return The connection timeout bound. Default value is 5000 ms. (5
     * seconds)
     */
    int connectionTimeout() const;

    /*!
     * \brief Set the server connection timeout bound.
     * \param msec Time out bound. The unit is ms.
     */
    void setConnectionTimeout(int msec);

    /*!
     * \brief Connect to the protocol host server with the given configuration.
     * \return If the client could connect to the server successfully, return
     * true.
     */
    virtual bool connectToHost()=0;

    /*!
     * \brief Login the mail server with given parameter.
     * \return If login success, return true.
     */
    virtual bool login()=0;

    /*!
     * \brief Quit the mail server from the login state.
     */
    virtual bool quit()=0;

    /*!
     * \brief Get whether the client is still connected with the server.
     * \return If the client is still connected, it will return true.
     */
    bool isConnected() const;

signals:
    /*!
     * \brief This signal will be emitted when any error happened.
     * \param e Error type.
     */
    void clientError(KMAbstractMailClient::MailClientError e);

    /*!
     * \brief When client state changed, this signal will be emitted.
     */
    void clientStateChanged();

public slots:

protected:
    /*!
     * \brief Wait for server response, and get the response data from server.
     * After called this function, response code and response text will be
     * updated.
     * \return If host server responsed our request and all the responsed data
     * updated successfully, it will return true.
     */
    bool waitForResponse();

    /*!
     * \brief Send message to the host server.
     * \param text The message data.
     * \return If send the message timeout, it will return false. If the text
     * could be send in time, return true;
     */
    bool sendMessage(const QString &text);

    /*!
     * \brief Get the socket pointer.
     * \return The socket pointer. Before the connection type is set, it will be
     * nullptr.
     */
    QTcpSocket *socket() const;

    /*!
     * \brief Get the setted password.
     * \return The password. It's a null string before you set it.
     */
    QString password() const;

    /*!
     * \brief Update the response code according to the text.
     * \param responseText The response text.
     */
    virtual void updateResponseCode(const QString &responseText)=0;

    /*!
     * \brief Update the response code according to the response text
     * \param responseCode The new response code.
     */
    void setResponseCode(int responseCode);

    /*!
     * \brief Get the response text from server.
     * \return The response text. Before you send any message to server, this
     * function will return a null string.
     */
    QString responseText() const;

    /*!
     * \brief When the message reaches a special format, wait for response will
     * ended to fetch message. This filter will be used to filter the message.
     * \return If the message is the one we want, return true.
     */
    virtual bool messageFilter();

private:
    QString m_host, m_user, m_password, m_responseText;
    quint16 m_port;
    QTcpSocket *m_socket;
    ConnectionType m_connectionType;
    AuthMethod m_authMethod;
    int m_connectionTimeout, m_responseTimeout, m_sendMessageTimeout,
        m_responseCode;
};

#endif // KMABSTRACTMAILCLIENT_H
