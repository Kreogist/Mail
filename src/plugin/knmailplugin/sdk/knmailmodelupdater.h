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

#ifndef KNMAILMODELUPDATER_H
#define KNMAILMODELUPDATER_H

#include <QtConcurrent/QtConcurrent>

#include <QObject>

/*!
 * \def knMailModelUpdater
 * The global instance of the model updater.
 */
#define knMailModelUpdater (KNMailModelUpdater::instance())

class KNMailAccount;
class KNMailModel;
/*!
 * \brief The KNMailModelUpdater class provides the ability to update a part of
 * the folder model.
 */
class KNMailModelUpdater : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the global instance.
     * \return The instance pointer.
     */
    static KNMailModelUpdater *instance();

    /*!
     * \brief Initial the receiver updater object.
     * \param workingThread The object working thread.
     */
    static void initial(QThread *workingThread);

signals:

public slots:
    /*!
     * \brief Update one folder's content.
     * \param account The account class object.
     * \param folder The folder model pointer.
     * \param startPosition Start position of the update position.
     * \param endPosition End position of the update position.
     */
    void updateFolder(KNMailAccount *account, KNMailModel *folder,
                      int startPosition, int endPosition);

    /*!
     * \brief Update one folder's content. This will check the future watch and
     * launch the update code in a different thread.
     * \param account The account class object.
     * \param folder The folder model pointer.
     * \param startPosition Start position of the update position.
     * \param endPosition End position of the update position.
     */
    void startUpdateFolder(KNMailAccount *account, KNMailModel *folder,
                           int startPosition, int endPosition);

private:
    static KNMailModelUpdater *m_instance;
    explicit KNMailModelUpdater(QObject *parent = 0);
    KNMailModelUpdater(const KNMailModelUpdater &);
    KNMailModelUpdater(KNMailModelUpdater &&);
    QFuture<void> m_updateThread;
    QFutureWatcher<void> m_updateState;
};

#endif // KNMAILMODELUPDATER_H
