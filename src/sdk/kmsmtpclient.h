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

#ifndef KMSMTPCLIENT_H
#define KMSMTPCLIENT_H

#include "kmabstractsenderclient.h"

/*!
 * \brief The KMSmtpClient class provides the access for SMTP for E-mail sending
 * and package it into a single object.\n
 * Most of the codes is from SmtpClient-for-Qt by bluetiger9. Project is hosted
 * on Github, you can access it here:
 *      https://github.com/bluetiger9/SmtpClient-for-Qt\n
 * We change some of the codes to fit the standard of our coding style.
 */
class KMSmtpClient : public KMAbstractSenderClient
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMSmtpClient with given parent.
     * \param parent The parent object.
     */
    explicit KMSmtpClient(QObject *parent = 0);

    /*!
     * \brief Construct a KMSmtpClient and initial parameters.
     * \param host Set the host url. The default value is localhost.
     * \param port Set the SMTP connection port. The default value is 25.
     * \param ct Set the connection type. The default value is TcpConnection.
     * \param parent The parent object.
     */
    KMSmtpClient(const QString &host = "localhost",
                 quint16 port = 25,
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

protected:
    /*!
     * \brief Reimplemented from KMAbstractMailClient::updateResponseCode().
     */
    void updateResponseCode(const QString &responseText) Q_DECL_OVERRIDE;

public slots:
    /*!
     * \brief Reimplemented from KMAbstractMailClient::sendMail().
     */
    void sendMail(KMMailBody *mail) Q_DECL_OVERRIDE;

private:
    /*!
     * \brief This funciton is a second package function. It will first wait for
     * response from server. If the server is not response it will simply return
     * false. \n
     * If the server response, check the response code. If response code is not
     * the one we prefer, function will emit a error signal and return false.
     * The error if the failed error.\n
     * If there's no error happend, returned true.
     * \param captureCode Response code which will be captured.
     * \param failedError When the response code is not prefer, the error type.
     * \return When the capture code is the prefer one, return true.
     */
    inline bool waitAndCheckResponse(
            int captureCode,
            KMAbstractMailClient::MailClientError failedError);
};

#endif // KMSMTPCLIENT_H
