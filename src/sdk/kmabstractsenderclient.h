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

#ifndef KMABSTRACTSENDERCLIENT_H
#define KMABSTRACTSENDERCLIENT_H

#include "kmabstractmailclient.h"

class KMMailBody;
/*!
 * \brief The KMAbstractSenderClient class provides all the ports of a sender
 * client should realized. For all the sender protocol, you have to implemented
 * from this class and realized all the virtual functions.
 */
class KMAbstractSenderClient : public KMAbstractMailClient
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KMAbstractSenderClient object with given parent.
     * \param parent The parent object pointer.
     */
    KMAbstractSenderClient(QObject *parent = 0):KMAbstractMailClient(parent){}

public slots:
    /*!
     * \brief Send mail content via sender client.
     * \param mail The mail content pointer.
     */
    virtual void sendMail(KMMailBody *mail)=0;
};

#endif // KMABSTRACTSENDERCLIENT_H
