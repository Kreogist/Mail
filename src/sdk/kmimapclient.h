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

#ifndef KMIMAPCLIENT_H
#define KMIMAPCLIENT_H

#include "kmabstractreceiverclient.h"

/*!
 * \brief The KMImapClient class provide the ability to receive E-mail from a
 * server which support IMAP protocol.
 */
class KMImapClient : public KMAbstractReceiverClient
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KMImapClient object.
     * \param parent The parent object.
     */
    explicit KMImapClient(QObject *parent = 0);

    /*!
     * \brief Construct a KMImapClient and initial parameters.
     * \param host Set the host url. The default value is localhost.
     * \param port Set the Imap connection port. The default value is 143.
     * \param ct Set the connection type. The default value is TcpConnection.
     * \param parent The parent object.
     */
    KMImapClient(const QString &host = "localhost",
                 quint16 port = 143,
                 ConnectionType ct = TcpConnection,
                 QObject *parent = 0);

    /*!
     * \brief Reimplemented from KMAbstractMailClient::connectToHost().
     */
    bool connectToHost() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KMAbstractMailClient::login().
     */
    bool login() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KMAbstractMailClient::quit().
     */
    bool quit() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KMAbstractMailClient::getMailList().
     */
    void getMailList() Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KMAbstractMailClient::updateResponseCode().
     */
    void updateResponseCode(const QString &responseText) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KMAbstractMailClient::messageFilter().
     */
    bool messageFilter() Q_DECL_OVERRIDE;

private:
    inline bool waitAndCheckResponse();
    inline bool sendImapMessage(const QString &command);
    quint64 m_counter;
};

#endif // KMIMAPCLIENT_H
