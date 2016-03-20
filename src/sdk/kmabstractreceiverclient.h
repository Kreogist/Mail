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

#ifndef KMABSTRACTRECEIVERCLIENT_H
#define KMABSTRACTRECEIVERCLIENT_H

#include "kmabstractmailclient.h"

/*!
 * \brief The KMAbstractReceiverClient class provides all the ports of a
 * receiver client should realized. For all the receiver protocol, you have to
 * implemented from this object and realized all the pure virtual function.
 */
class KMAbstractReceiverClient : public KMAbstractMailClient
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcuct a KMAbstractReceiverClient object with given parent.
     * \param parent The parent object.
     */
    KMAbstractReceiverClient(QObject *parent = 0):KMAbstractMailClient(parent){}

    /*!
     * \brief Get client working state.
     * \return Working state. If client is working, return true.
     */
    bool isWorking() const
    {
        return m_isWorking;
    }

signals:

public slots:
    /*!
     * \brief getMailList
     */
    virtual void getMailList()=0;

protected:
    /*!
     * \brief Update the working state.
     * \param isWorking Working state. When client is working, state will be
     * true.
     */
    void setWorkingState(bool isWorking)
    {
        //Save the working state.
        m_isWorking=isWorking;
    }

private:
    bool m_isWorking;
};

#endif // KMABSTRACTRECEIVERCLIENT_H

