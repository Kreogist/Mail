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

#ifndef KNMAILSENDERMANAGER_H
#define KNMAILSENDERMANAGER_H

#include <QObject>

#define knMailSenderManager (KNMailSenderManager::instance())

class KNMimePart;
class KNMailAccount;
/*!
 * \brief The KNMailSenderManager class provides the management of all the send
 * protocols. It has the list which is waiting to be sent.
 */
class KNMailSenderManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Create the global instance.
     * \param workingThread The working object thread.
     */
    static void initial(QThread *workingThread);

    /*!
     * \brief Get the global instance pointer.
     * \return The instance pointer.
     */
    static KNMailSenderManager *instance();

signals:
    /*!
     * \brief This function will only be used internally, don't call this
     * function out size. Ask for update the next item.
     */
    void requireUpdateNext();

public slots:
    void sendMail(KNMimePart *mimePart,
                  KNMailAccount *account,
                  const QString &receiver);

private slots:
    void onUpdateNextItem();

private:
    static KNMailSenderManager *m_instance;
    explicit KNMailSenderManager(QObject *parent = 0);
    KNMailSenderManager(const KNMailSenderManager &);
    KNMailSenderManager(KNMailSenderManager &&);
    struct SendMailItem
    {
        KNMimePart *mimePart;
        KNMailAccount *account;
        QString receiver;
    };
    QList<SendMailItem> m_mailItemList;
    bool m_isWorking;
};

#endif // KNMAILSENDERMANAGER_H
