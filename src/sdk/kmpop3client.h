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

#ifndef KMPOP3CLIENT_H
#define KMPOP3CLIENT_H

#include "kmabstractreceiverclient.h"

/*!
 * \brief The KMPop3Client provides the ability to receive E-mail from a POP3
 * protocol server.\n
 * Most of the code from this client is from:\n
 *      http://www.oschina.net/code/snippet_188882_32619
 */
class KMPop3Client : public KMAbstractReceiverClient
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KMPop3Client object with given parent.
     * \param parent The parent object.
     */
    explicit KMPop3Client(QObject *parent = 0);

    /*!
     * \brief Construct a KMPop3Client and initial parameters.
     * \param host Set the host url. The default value is localhost.
     * \param port Set the POP3 connection port. The default value is 110.
     * \param ct Set the connection type. The default value is TcpConnection.
     * \param parent The parent object.
     */
    KMPop3Client(const QString &host = "localhost",
                 quint16 port = 110,
                 ConnectionType ct = TcpConnection,
                 QObject *parent = 0);

    /*!
     * \brief Reimplemented from KMAbstractMailClient::connectToHost().
     */
    bool connectToHost() Q_DECL_OVERRIDE;

    /*!
     * \brief This is a quick login function with set the user name, password
     * and authentication method for once.
     * \param userName E-mail user name.
     * \param password E-mail password.
     * \param method Login authentication method.
     * \return If login success, return true.
     */
    inline bool login(const QString &userName,
                      const QString &password,
                      AuthMethod method = AuthLogin)
    {
        //Set the login information.
        setUserName(userName);
        setPassword(password);
        setAuthMethod(method);
        //Do login.
        return login();
    }

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

private:
    inline bool waitAndCheckResponse();
};

#endif // KMPOP3CLIENT_H
