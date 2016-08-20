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

#include <QList>

#include "knmailutil.h"

#include <QAbstractTableModel>

using namespace MailUtil;

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
     * \brief Get the physical folder path on the hard drive. It should be a
     * relative folder position which should combine with the parent folder path
     * from mail user configuration.
     * \return  The folder relative position. Default it is an empty string.
     */
    QString folderPath() const;

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
     * \brief Set the folder display name.
     * \param folderName The folder display name.
     */
    void setFolderName(const QString &folderName);

    /*!
     * \brief Get the default folder index of the model describe. If the index
     * is -1, it will be treated as a custom folder.
     * \return The folder index, default is -1.
     */
    int defaultFolderIndex() const;

signals:

public slots:
    /*!
     * \brief Set the default folder index.
     * \param defaultFolderIndex The folder index.
     */
    void setDefaultFolderIndex(int defaultFolderIndex);

    /*!
     * \brief Set the folder path position.
     * \param folderPath The folder path position.
     */
    void setFolderPath(const QString &folderPath);

    /*!
     * \brief Parse the mail item at a specific range.
     * \param start The start position.
     * \param end The end position.
     */
    void loadItem(int start, int end);

private:
    QList<KNMailListItem> m_itemList;
    QString m_folderPath, m_folderName;
    int m_defaultFolderIndex;
};

#endif // KNMAILMODEL_H
