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
#ifndef KMMAILLISTMODEL_H
#define KMMAILLISTMODEL_H

#include <QList>

#include "kmmailutil.h"

#include <QAbstractListModel>

/*!
 * \brief The KMMailListModel class provides the E-mail list data. All the
 * E-mail collections could be describe as a mail list.
 */
class KMMailListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailListModel object.
     * \param parent The parent widget.
     */
    explicit KMMailListModel(QObject *parent = 0);

    /*!
     * \brief Reimplemented from QAbstractListModel::rowCount().
     */
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractListModel::data().
     */
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    /*!
     * \brief Append one row to the end of the list.
     * \param item The mail item.
     */
    void appendRow(const KMMailUtil::MailListItem &item);

signals:

public slots:

private:
    QList<KMMailUtil::MailListItem> m_mailLists;
};

#endif // KMMAILLISTMODEL_H
