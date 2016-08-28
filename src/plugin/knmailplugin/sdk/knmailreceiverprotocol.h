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
#ifndef KNMAILRECEIVERPROTOCOL_H
#define KNMAILRECEIVERPROTOCOL_H

#include "knmailprotocol.h"

class KNMailModel;
/*!
 * \brief The KNMailReceiverProtocol class provides the receiver abstract
 * protocol. It will manage the update configure of the protocol and the data
 * related to operating with the model.
 */
class KNMailReceiverProtocol : public KNMailProtocol
{
    Q_OBJECT
public:
    explicit KNMailReceiverProtocol(QObject *parent = 0);

signals:

public slots:
    /*!
     * \brief Update all the folder status of the account.
     * \return If the protocol update the status successfully, then return true.
     */
    virtual bool updateFolderStatus()=0;

    /*!
     * \brief Update a specfic folder content.
     * \param folder The folder model pointer.
     * \return If we update the folder successfully, then return true, else
     * return false.
     */
    virtual bool updateFolder(KNMailModel *folder)=0;

protected:
    /*!
     * \brief Reimplemented from KNMailProtocol::updateProtocolConfig().
     */
    void updateProtocolConfig() Q_DECL_OVERRIDE;
};

#endif // KNMAILRECEIVERPROTOCOL_H
