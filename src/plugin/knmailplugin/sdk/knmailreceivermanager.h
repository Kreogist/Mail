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
#ifndef KNMAILRECEIVERMANAGER_H
#define KNMAILRECEIVERMANAGER_H

#include <QObject>

/*!
 * \def knMailReceiverManager
 * A global pointer of the mail receiver manager.
 */
#define knMailReceiverManager (KNMailReceiverManager::instance())

/*!
 * \brief The KNMailReceiverManager class will manage all the receive client,
 * and automatically update all the receiver client in the queue.
 */
class KNMailReceiverManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the global instance of the receiver manager.
     * \return The instance pointer.
     */
    static KNMailReceiverManager *instance();

    /*!
     * \brief Create the global instance.
     * \param workingThread The working object thread.
     */
    static void initial(QThread *workingThread);

signals:

public slots:

private:
    static KNMailReceiverManager *m_instance;
    explicit KNMailReceiverManager(QObject *parent = 0);
    KNMailReceiverManager(const KNMailReceiverManager &);
    KNMailReceiverManager(KNMailReceiverManager &&);
};

#endif // KNMAILRECEIVERMANAGER_H
