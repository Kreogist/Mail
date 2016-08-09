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

#ifndef KMMAILRECEIVERMANAGER_H
#define KMMAILRECEIVERMANAGER_H

#include <QList>
#include <QMutex>

#include <QObject>

#define kmReceiverManager (KMMailReceiverManager::instance())

class KNMailAccount;
/*!
 * \brief The KMMailReceiverManager class will manage all the receive client.
 * And automatically update all the receiver client.
 */
class KMMailReceiverManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the instance of the receiver manager.
     * \return The global receiver instance pointer.
     */
    static KMMailReceiverManager *instance();

    /*!
     * \brief Initial the receiver manager.
     */
    static void initial(QObject *parent);

signals:
    void requireProcessNext();

public slots:
    void appendMail(KNMailAccount *accout);

private slots:
    void onProcessNext();

private:
    explicit KMMailReceiverManager(QObject *parent = 0);
    static KMMailReceiverManager *m_instance;
    QList<KNMailAccount *> m_mailRecevieList;
    QMutex m_queueLock;
    bool m_isWorking = true;
};

#endif // KMMAILRECEIVERMANAGER_H
