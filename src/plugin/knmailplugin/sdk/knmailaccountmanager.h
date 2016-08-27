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
#ifndef KNMAILACCOUNTMANAGER_H
#define KNMAILACCOUNTMANAGER_H

#include <QList>

#include "knmailutil.h"

#include <QAbstractListModel>

using namespace MailUtil;

/*!
 * \def knMailAccountManager
 * A global pointer referring to the unique mail account manager instance.
 */
#define knMailAccountManager (KNMailAccountManager::instance())

class KNConfigure;
class KNMailAccount;
/*!
 * \brief The KNMailAccountManager class provides a unique interface to manage
 * all the account objects.
 */
class KNMailAccountManager : public QAbstractListModel
{
    Q_OBJECT
public:
    ~KNMailAccountManager();

    /*!
     * \brief Get the account manager global instance.
     * \return The account manager global instance.
     */
    static KNMailAccountManager *instance();

    /*!
     * \brief Initial the account manager with the given parent object.
     * \param parent The parent object pointer.
     */
    static void initial(QObject *parent);

    /*!
     * \brief Append one account object to the end of the manager. The manager
     * will take the ownership of the account object.
     * \param account The mail account object.
     */
    void appendAccount(KNMailAccount *account);

    /*!
     * \brief Get the account object pointer via the account index.
     * \param accountIndex The account index.
     * \return Get the account object pointer.
     */
    KNMailAccount *account(int accountIndex);

    /*!
     * \brief Get the managed account number.
     * \return The account number in the manage list.
     */
    int accountCount();

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent=QModelIndex()) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

signals:
    /*!
     * \brief When one account is appended to the account manager, this signal
     * will be emitted.
     * \param account The account index.
     */
    void accountAppended(int accountIndex);

public slots:

private:
    inline KNMailProtocolConfig toConfig(QJsonObject *configObject);
    static KNMailAccountManager *m_instance;
    explicit KNMailAccountManager(QObject *parent = 0);

    QList<KNMailAccount *> m_accountList;
    KNConfigure *m_accountConfigure;
};

#endif // KNMAILACCOUNTMANAGER_H
