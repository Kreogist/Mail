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
#ifndef KMMAILACCOUNT_H
#define KMMAILACCOUNT_H

#include <QList>
#include <QObject>

#include "kmmailutil.h"

using namespace MailUtil;

class KMMailListModel;
/*!
 * \brief The KMMailAccount class
 */
class KMMailAccount : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailAccount object with given parent.
     * \param parent The parent object.
     */
    explicit KMMailAccount(QObject *parent = 0);
    ~KMMailAccount();

    /*!
     * \brief accountProperty
     * \param propertyIndex
     * \return
     */
    QString accountProperty(int propertyIndex) const;

    /*!
     * \brief systemFolder
     * \param folderIndex
     * \return
     */
    KMMailListModel *systemFolder(int folderIndex);

    /*!
     * \brief customFolder
     * \param customerIndex
     * \return
     */
    KMMailListModel *customFolder(int customerIndex);

    QString dirName() const;

    void setDirName(const QString &dirName);

signals:
    void propertyChanged();

public slots:
    /*!
     * \brief setAccountProperty
     * \param propertyIndex
     * \param value
     */
    void setAccountProperty(int propertyIndex, const QString &value);

    /*!
     * \brief clearCustomerFolder
     */
    void clearCustomerFolder();

private:
    QList<KMMailListModel *> m_customFolder;
    KMMailListModel *m_systemFolder[MailSystemFoldersCount];
    QString m_properties[MailAccountPropertiesCount];
    QString m_dirName;
};

#endif // KMMAILACCOUNT_H
