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
    /*!
     * \brief Construct a KNMailReceiverProtocol object.
     * \param parent The parent object.
     */
    KNMailReceiverProtocol(QObject *parent = 0):KNMailProtocol(parent){}

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

    /*!
     * \brief Update the folder item content.
     * \param folder The folder model.
     * \param startPosition The start position of the item.
     * \param endPosition The end position of the item.
     * \return If all the data is cached, return true.
     */
    virtual bool updateFolderContent(KNMailModel *folder,
                                     int startPosition,
                                     int endPosition)=0;

    /*!
     * \brief Download one single mail from the web account.
     * \param folder The folder model.
     * \param mailIndex The index of the mail.
     * \return If we could receive the mail from the account, return true.
     */
    virtual bool downloadMail(KNMailModel *folder, int mailIndex)=0;

protected:
    /*!
     * \brief Reimplemented from KNMailProtocol::updateProtocolConfig().
     */
    void updateProtocolConfig() Q_DECL_OVERRIDE;
};

#endif // KNMAILRECEIVERPROTOCOL_H
