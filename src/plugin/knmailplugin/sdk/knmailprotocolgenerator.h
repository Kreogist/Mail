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
#ifndef KNMAILPROTOCOLGENERATOR_H
#define KNMAILPROTOCOLGENERATOR_H

#include <QString>

class KNMailProtocol;
/*!
 * \brief The KNMailProtocolGenerator class provides the basic ports for a
 * protocol generator, this is the abstract factory for the protocol.
 */
class KNMailProtocolGenerator
{
public:
    /*!
     * \brief Construct a KNMailProtocolGenerator object.
     */
    KNMailProtocolGenerator(){}
    virtual ~KNMailProtocolGenerator(){}

    /*!
     * \brief Get the name of the generated protocol.
     * \return The protocol name, all in lower case.
     */
    virtual QString protocolName() const=0;

    /*!
     * \brief Generate a named protocol.
     * \return The generated protocol pointer.
     */
    virtual KNMailProtocol *generateProtocol()=0;

    /*!
     * \brief Get whether the protocol is a protocol used for receiving mails.
     * \return If it is an receving mail protocol realization, return true.
     */
    virtual bool isReceiverProtocol() const=0;

    /*!
     * \brief Get whether the protocol is a protocol used for sending mails, it
     * is simply reverse the receiver's response.
     * \return If it is not an receving mail protocol realization, return true.
     */
    bool isSenderProtocol() const
    {
        return !isReceiverProtocol();
    }
};

#endif // KNMAILPROTOCOLGENERATOR_H
