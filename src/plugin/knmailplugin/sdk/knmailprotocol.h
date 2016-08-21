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
#ifndef KNMAILPROTOCOL_H
#define KNMAILPROTOCOL_H


#include "knmailutil.h"

#include <QObject>

#ifdef Q_OS_WIN
#include <QEventLoop>
#endif

using namespace MailUtil;

class QTcpSocket;
class KNMailAccount;
/*!
 * \brief The KNMailProtocol class provides the basic ports for mail protocol.
 * All the mail service communication protocol which has to communicate with the
 * server must implemented from this class.
 */
class KNMailProtocol : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailProtocol object.
     * \param parent The parent object.
     */
    explicit KNMailProtocol(QObject *parent = 0);
    ~KNMailProtocol();

    /*!
     * \brief Get the connection timeout. Unit is msecond.
     * \return The connection timeout value. Default is 5000. (5 seconds)
     */
    int connectionTimeout() const
    {
        return m_connectionTimeout;
    }

    /*!
     * \brief Get the last error of the protocol.
     * \return The last error. Default it is -1.
     */
    int lastError() const
    {
        return m_lastError;
    }

    /*!
     * \brief Get the response timeout. Unit is msecond.
     * \return The response timeout value. Default is 5000. (5 seconds)
     */
    int responseTimeout() const
    {
        return m_responseTimeout;
    }

    /*!
     * \brief Get mail account pointer.
     * \return The mail account information pointer. Default is nullptr.
     */
    KNMailAccount *account() const;

signals:

public slots:
    /*!
     * \brief Set mail account to the configuration.
     * \param account Account object.
     */
    void setAccount(KNMailAccount *account);

    /*!
     * \brief Connect to the host server.
     * \return If the config is correct and the server is connected, this will
     * return true. Or else, false for any error.
     */
    virtual bool connectToHost()=0;

    /*!
     * \brief Login to the host server.
     * \return If the protocol could connect to host server and it can login
     * with the account settings, then return true. Or else return false for
     * any error.
     */
    virtual bool login()=0;

    /*!
     * \brief Set connection timeout value.
     * \param connectionTimeout New timeout value.
     */
    void setConnectionTimeout(int connectionTimeout);

    /*!
     * \brief Set response timeout value.
     * \param responseTimeout New response timeout value.
     */
    void setResponseTimeout(int responseTimeout);

protected:
    /*!
     * \brief Get the communication socket pointer.
     * \return The socket pointer. It will be nullptr before the configuration
     * is updated.
     */
    QTcpSocket *socket() const;

    /*!
     * \brief Set last error code.
     * \param errorCode The error code.
     */
    void setLastError(int errorCode);

    /*!
     * \brief Start socket communication with the server. It will change the
     * communication type according to the protocol type.
     * \param config Protocol connection config.
     * \return If the socket is null, return false. Or else, if it start
     * connection, then return true.
     */
    bool startConnection(const KNMailProtocolConfig &config);

    /*!
     * \brief Wait for socket to get the response of connection from the server.
     * \return If connected to server successfully, then return true.
     */
    bool waitForConnection();

    /*!
     * \brief Update the protocol config to fit the new account information.
     */
    virtual void updateProtocolConfig()=0;

    /*!
     * \brief Update the socket via the connection type.
     * \param type The connection type.
     */
    void setSocketType(MailProtocolSocket type);

    /*!
     * \brief Send one line of message via the socket. This function will add
     * return and next line to the end of the command (CRLF).
     * \param message The line message.
     * \return If the data is written, it will return true.
     */
    bool sendMessage(const QString &message);

    /*!
     * \brief Wait for socket to get the response. This function combine the
     * result into a single string.
     * \param responseText The cache to output the response data.
     * \return If the socket response the data. It will return true.
     */
    bool waitForResponse(QByteArray *responseText = nullptr);

    /*!
     * \brief Wait for socket to get the response. However, this function won't
     * combine the response to one string, it will save the response in a string
     * list.
     * \param responseText The cache to save the response data.
     * \return If the socket response the data. It will return true.
     */
    bool waitForResponse(QList<QByteArray> *responseText);

private:
    inline bool waitForReadyRead();
#ifdef Q_OS_WIN
    QEventLoop m_waitWriteLoop, m_waitReadLoop, m_waitConnectLoop;
#endif
    QTcpSocket *m_socket;
    KNMailAccount *m_account;
    int m_lastError, m_connectionTimeout, m_responseTimeout, m_sendTimeout;
#ifdef Q_OS_WIN
    bool m_dataWritten, m_dataReadyRead, m_socketConnected;
#endif
};

#endif // KNMAILPROTOCOL_H
