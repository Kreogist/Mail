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
#ifndef KNMAILPROTOCOLMANAGER_H
#define KNMAILPROTOCOLMANAGER_H

#include <QMap>

#include <QObject>

/*!
 * \def knMailProtocolManager
 * The global instance pointer of the protocol factory manager.
 */
#define knMailProtocolManager (KNMailProtocolManager::intance())

class KNMailSenderProtocol;
class KNMailReceiverProtocol;
class KNMailProtocolGenerator;
/*!
 * \brief The KNMailProtocolManager class manage all the factory class of the
 * protocol. It could generate the protocol via given the lower case name of the
 * protocol, e.g. imap, pop3 and smtp.
 */
class KNMailProtocolManager : public QObject
{
    Q_OBJECT
public:
    ~KNMailProtocolManager();

    /*!
     * \brief Get the intance of the manager.
     * \return The manager instance pointer.
     */
    static KNMailProtocolManager *intance();

    /*!
     * \brief Initial the protocol manager.
     */
    static void initial();

    /*!
     * \brief Generate a receiver protocol object class.
     * \param name The protocol name.
     * \return The protocol object pointer. If the protocol cannot be generated,
     * then a nullptr pointer will be returned.
     */
    KNMailReceiverProtocol *generateReceiverProtocol(const QString &name);

    /*!
     * \brief Generate a sender protocol object class.
     * \param name The protocol name.
     * \return The protocol object pointer. If the protocol cannot be generated,
     * then a nullptr pointer will be returned.
     */
    KNMailSenderProtocol *generateSenderProtocol(const QString &name);

signals:

public slots:
    /*!
     * \brief Append one factory object class to the map.
     * \param generator The generator object pointer.
     */
    void appendFactory(KNMailProtocolGenerator *generator);

private:
    static KNMailProtocolManager *m_instance;

    explicit KNMailProtocolManager(QObject *parent = 0);
    KNMailProtocolManager(const KNMailProtocolManager &);
    KNMailProtocolManager(KNMailProtocolManager &&);

    QMap<QString, KNMailProtocolGenerator *> m_receiverMap;
};

#endif // KNMAILPROTOCOLMANAGER_H
