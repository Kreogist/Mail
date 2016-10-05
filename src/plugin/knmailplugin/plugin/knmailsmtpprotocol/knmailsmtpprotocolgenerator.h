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

#ifndef KNMAILSMTPPROTOCOLGENERATOR_H
#define KNMAILSMTPPROTOCOLGENERATOR_H

#include "knmailsmtpprotocol.h"

#include "knmailprotocolgenerator.h"

/*!
 * \brief The KNMailSmtpProtocolGenerator class provides the factory for the
 * SMTP communication protcol.
 */
class KNMailSmtpProtocolGenerator : public KNMailProtocolGenerator
{
public:
    /*!
     * \brief Construct a KNMailSmtpProtocolGenerator object.
     */
    KNMailSmtpProtocolGenerator():KNMailProtocolGenerator(){}

    /*!
     * \brief Reimplemented from KNMailProtocolGenerator::protocolName().
     */
    QString protocolName() const Q_DECL_OVERRIDE
    {
        return QString("smtp");
    }

    /*!
     * \brief Reimplemented from KNMailProtocolGenerator::generateProtocol().
     */
    KNMailProtocol *generateProtocol() Q_DECL_OVERRIDE
    {
        return new KNMailSmtpProtocol();
    }

    /*!
     * \brief Reimplemented from KNMailProtocolGenerator::isReceiverProtocol().
     */
    bool isReceiverProtocol() const
    {
        return false;
    }
};

#endif // KNMAILSMTPPROTOCOLGENERATOR_H
