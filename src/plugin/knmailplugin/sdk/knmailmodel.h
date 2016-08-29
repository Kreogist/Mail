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
#ifndef KNMAILMODEL_H
#define KNMAILMODEL_H

#include <QJsonObject>
#include <QJsonArray>
#include <QList>

#include "knmailutil.h"

#include <QAbstractTableModel>

using namespace MailUtil;

class KNMailAccount;
/*!
 * \brief The KNMailModel class provides the mail information of a mail account
 * folder.
 */
class KNMailModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailModel object with given parent object.
     * \param parent The parent object.
     */
    explicit KNMailModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractTableModel::rowCount().
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const
    Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::columnCount().
     */
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractTableModel::headerData().
     */
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    /*!
     * \brief Append one item to the end of the model.
     * \param item The mail item.
     */
    void appendRow(const KNMailListItem &item);

    /*!
     * \brief Append more than one item to the end of the model once.
     * \param items The mail item list.
     */
    void appendRows(const QList<KNMailListItem> &items);

    /*!
     * \brief Get the name of the folder.
     * \return The folder name.
     */
    QString folderName() const;

    /*!
     * \brief Get the default folder index of the model describe. If the index
     * is -1, it will be treated as a custom folder.
     * \return The folder index, default is -1.
     */
    int defaultFolderIndex() const;

    /*!
     * \brief Get the folder name saved on server.
     * \return The folder name on server.
     */
    QString serverName() const;

    /*!
     * \brief Check whether the data of the model from start to end need cache.
     * \param startPosition The start position.
     * \param endPosition The end position.
     * \return If any data is not cached, return true.
     */
    bool needCache(int startPosition, int endPosition);

    /*!
     * \brief Check whether a specific item is already cached.
     * \param position The item position.
     * \return If the item is cached, return true.
     */
    bool isItemCached(int position);

    /*!
     * \brief Get the uid of the mail at the specific position.
     * \param position The mail position in the model.
     * \return The uid of the mail.
     */
    int uid(int position);

    /*!
     * \brief Get the managed account. It is actuall the parent of this model,
     * however, the multithread is forbidden to operate the parent. So we used
     * this.
     * \return The parent account.
     */
    KNMailAccount *managedAccount() const;

    /*!
     * \brief Get the item at position i.
     * \param position The position of the item in the list.
     * \return The item at the position.
     */
    KNMailListItem item(int position) const;

signals:
    /*!
     * \brief When the model is synced with server, this signal will be emitted.
     */
    void modelUpdated();

public slots:
    /*!
     * \brief Update an E-mail item at the position.
     * \param position The position of the item.
     * \param item The item of the position.
     */
    void updateItem(int position, const KNMailListItem &item);

    /*!
     * \brief Set the folder name saved on server.
     * \param serverName The folder name on server.
     */
    void setServerName(const QString &serverName);

    /*!
     * \brief Set the default folder index.
     * \param defaultFolderIndex The folder index.
     */
    void setDefaultFolderIndex(int defaultFolderIndex);

    /*!
     * \brief Set the folder display name.
     * \param folderName The folder display name.
     */
    void setFolderName(const QString &folderName);

    /*!
     * \brief Load mail folder content from account folder.
     * \param accountFolder The account folder.
     */
    void loadFromFolder(const QString &accountFolder);

    /*!
     * \brief Save mail content to specific folder.
     * \param accountFolder The account folder.
     */
    void saveToFolder(const QString &accountFolder);

    /*!
     * \brief Clear all the content in the model.
     */
    void reset();

    /*!
     * \brief Remove the model content on the hard drive.
     * \param accountFolder The account folder.
     */
    void removeModelContent(const QString &accountFolder);

    /*!
     * \brief Update current folder's mail content via the new uid list.
     * \param accountFolder The account folder.
     * \param uidList The mail uid list.
     */
    void updateUidList(const QString &accountFolder, QList<int> *uidList);

    /*!
     * \brief Set the managed account object pointer.
     * \param managedAccount The account object pointer.
     */
    void setManagedAccount(KNMailAccount *managedAccount);

private:
    QList<KNMailListItem> m_itemList;
    QString m_folderPath, m_folderName, m_serverName;
    QJsonObject m_folderContent;
    QJsonArray m_itemArray;
    KNMailAccount *m_managedAccount;
    int m_defaultFolderIndex;
};

#endif // KNMAILMODEL_H
