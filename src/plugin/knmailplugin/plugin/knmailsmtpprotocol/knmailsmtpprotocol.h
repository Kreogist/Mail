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
<<<<<<< HEAD
 * along with this program; if not, write to the Free Software
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMAILSMTPPROTOCOL_H
#define KNMAILSMTPPROTOCOL_H

#include "knmailsenderprotocol.h"

/*!
 * \brief The KNMailSmtpProtocol class provides the SMTP protocol communication
 * for the client.
 */
class KNMailSmtpProtocol : public KNMailSenderProtocol
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailSmtpProtocol object.
     * \param The parent object.
     */
    explicit KNMailSmtpProtocol(QObject *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMailSendProtocol::connectToHost().
     */
    bool connectToHost() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailProtocol::login().
     */
    bool login() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplmented from KNMailSenderProtocol::sendMail().
     */
    bool sendMail(const QString &receiverAddress,
                  KNMimePart *mimePart) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplmented from KNMailSenderProtocol::logout().
     */
    bool logout() Q_DECL_OVERRIDE;

private:
    inline bool waitAndCheckResponse(int targetNumber);
    bool m_loginState;
};

#endif // KNMAILSMTPPROTOCOL_H
