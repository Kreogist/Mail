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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMAILSENDERPROTOCOL_H
#define KNMAILSENDERPROTOCOL_H

#include "knmailprotocol.h"

class KNMimePart;
/*!
 * \brief The KNMailSenderProtocol class provides the basic ports for the
 * sending protocols like SMTP.
 */
class KNMailSenderProtocol : public KNMailProtocol
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailSenderProtocol object.
     * \param parent The parent widget.
     */
    KNMailSenderProtocol(QObject *parent = 0) : KNMailProtocol(parent){}

signals:

public slots:
    /*!
     * \brief Send one mail in mime format.
     * \param mimePart The mime part data.
     * \return If it sends the mail successfully, return true, or else false for
     * failed.
     */
    virtual bool sendMail(const QString &receiverAddress,
                          KNMimePart *mimePart)=0;

    /*!
     * \brief Logout the protocol.
     * \return The protocol status.
     */
    virtual bool logout()=0;

protected:
    /*!
     * \brief Reimplemented from KNMailProtocol::updateProtocolConfig().
     */
    void updateProtocolConfig() Q_DECL_OVERRIDE;
};

#endif // KNMAILSENDERPROTOCOL_H
