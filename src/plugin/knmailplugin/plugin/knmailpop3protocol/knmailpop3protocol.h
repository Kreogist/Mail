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
#ifndef KNMAILPOP3PROTOCOL_H
#define KNMAILPOP3PROTOCOL_H

#include "knmailreceiverprotocol.h"

/*!
 * \brief The KNMailPop3Protocol class provides the ability to communicate with
 * the server using POP3 protocol.\n
 * This protocol is built as throwaway class. Set the protocol configure and the
 * user account information, it could use as the communication protocol.
 */
class KNMailPop3Protocol : public KNMailReceiverProtocol
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailPop3Protocol class.
     * \param parent The parent object.
     */
    explicit KNMailPop3Protocol(QObject *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMailProtocol::connectToHost().
     */
    bool connectToHost() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailProtocol::login().
     */
    bool login() Q_DECL_OVERRIDE;

protected:

private:
    inline bool waitAndCheckResponse();
    bool m_breakFlag;
};

#endif // KNMAILPOP3PROTOCOL_H
