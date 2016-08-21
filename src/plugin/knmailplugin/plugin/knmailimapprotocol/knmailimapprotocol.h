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
#ifndef KNMAILIMAPPROTOCOL_H
#define KNMAILIMAPPROTOCOL_H

#include "knmailreceiverprotocol.h"

/*!
 * \brief The KNMailImapProtocol class provides the ability to communicate with
 * the server using IMAP protocol.\n
 * Each IMAP protocol should complete only one operation, it is not built for
 * continously tracking with server.
 */
class KNMailImapProtocol : public KNMailReceiverProtocol
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailImapProtocol object.
     * \param parent The parent object.
     */
    explicit KNMailImapProtocol(QObject *parent = 0);

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

    /*!
     * \brief Reimplemented from KNMailProtocol::updateFolderStatus().
     */
    bool updateFolderStatus() Q_DECL_OVERRIDE;

private:
    inline bool waitAndCheckResponse(QList<QByteArray> *responseCache=nullptr);
    inline bool sendImapMessage(const QString &message)
    {
        //Update header.
        m_header="A"+QString::number(m_idCode++);
        //Send the message.
        return sendMessage(m_header+" "+message);
    }
    QString m_header;
    quint64 m_idCode;
    bool m_loginState;
};

#endif // KNMAILIMAPPROTOCOL_H
