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
#ifndef KMMAILACCOUNTMANAGER_H
#define KMMAILACCOUNTMANAGER_H

#include "kmmailutil.h"

#include <QObject>

#define kmMailAccount (KMMailAccountManager::instance())

using namespace MailUtil;

class KMMailAccount;
class KNConfigure;
/*!
 * \brief The KMMailAccountManager class
 */
class KMMailAccountManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief instance
     * \return
     */
    static KMMailAccountManager *instance();

    /*!
     * \brief initial
     * \param parent
     */
    static void initial(QObject *parent);

    /*!
     * \brief addMailAccount
     * \param account
     */
    void addMailAccount(KMMailAccount *account);

signals:
    /*!
     * \brief accountAppend
     * \param account
     */
    void accountAppend(KMMailAccount *account);

public slots:

    /*!
     * \brief loadConfigure
     */
    void loadConfigure();

    /*!
     * \brief saveConfigure
     */
    void saveConfigure();

private:
    static KMMailAccountManager *m_instance;
    explicit KMMailAccountManager(QObject *parent = 0);
    QList<KMMailAccount *> m_accountList;
    QString m_accountProperies[MailAccountPropertiesCount];
    KNConfigure *m_userConfigure;
};

#endif // KMMAILACCOUNTMANAGER_H
