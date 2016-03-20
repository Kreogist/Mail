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

#ifndef KMCONTACTLISTMODEL_H
#define KMCONTACTLISTMODEL_H

#include <QJsonArray>

#include <QAbstractListModel>

class QFileSystemWatcher;
/*!
 * \brief The KMContactListModel class will read the Kreogist Contact json
 * database and parse it as mail special model.
 */
class KMContactListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMContactListModel objec twith given parent object.
     * \param parent The parent object pointer.
     */
    explicit KMContactListModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Set the contact file path. Model will watch this file, when file
     * is modified, model will be updated automatically.
     * \return The contact file path.
     */
    QString contactFilePath() const;

    /*!
     * \brief Search E-mail address in the model.
     * \param mailAddress Target E-mail address.
     * \return The contact object. This object won't be updated when the
     * contanct reload. If we cannot find the mail address, it will return a
     * null JSON object.
     */
    QJsonObject searchEmailAddress(const QString &mailAddress);

signals:

public slots:
    /*!
     * \brief Set the contact file path in the system.\n
     * Notice: This function won't load the contact file.
     * \param contactFilePath The contact file path.
     */
    void setContactFilePath(const QString &contactFilePath);

    /*!
     * \brief Load contact content from specific file.
     */
    void loadContactFile();

    /*!
     * \brief Save contact content to contact file.
     */
    void saveContactFile();

private slots:
    void onActionContactFileChanged();

private:
    QJsonArray m_contactList;
    QString m_contactFilePath;
    QFileSystemWatcher *m_contactWatcher;
};

#endif // KMCONTACTLISTMODEL_H
