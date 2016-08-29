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
#ifndef KNMAILFOLDERPROXYMODEL_H
#define KNMAILFOLDERPROXYMODEL_H

#include <QSortFilterProxyModel>

/*!
 * \brief The KNMailFolderProxyModel class provides the proxy model for the
 * treeview model. It could show the different page of the mail model.
 */
class KNMailFolderProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailFolderProxyModel object.
     * \param parent The parent widget.
     */
    explicit KNMailFolderProxyModel(QObject *parent = 0);

    /*!
     * \brief Get the current page index.
     * \return The page index.
     */
    int pageIndex() const;

    /*!
     * \brief Set the page index.
     * \param pageIndex The page index.
     */
    void setPageIndex(int pageIndex);

    /*!
     * \brief Get the single page size.
     * \return The page size.
     */
    int pageSize() const;

    /*!
     * \brief Set the single mail page size.
     * \param pageSize The page size.
     */
    void setPageSize(int pageSize);

signals:
    /*!
     * \brief Ask to update the item area.
     * \param startPosition The start position. This index item should be
     * contained in the list.
     * \param endPosition The end position. This index shouldn't be contained.
     */
    void requireUpdate(int startPosition, int endPosition);

public slots:

protected:
    /*!
     * \brief Reimplemented from QSortFilterProxyModel::filterAcceptsRow().
     */
    bool filterAcceptsRow(int source_row,
                          const QModelIndex &source_parent) const
    Q_DECL_OVERRIDE;

private:
    inline void updateIndexes();
    int m_pageIndex, m_pageSize, m_pageStart, m_pageEnd;
};

#endif // KNMAILFOLDERPROXYMODEL_H
