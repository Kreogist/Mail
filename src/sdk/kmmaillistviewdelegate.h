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
#ifndef KMMAILLISTVIEWDELEGATE_H
#define KMMAILLISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

/*!
 * \brief The KMMailListViewDelegate class is used on the mail list view of the
 * left bar. This view delegate will only be used on the left bar list view.
 */
class KMMailListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailListViewDelegate object.
     * \param parent
     */
    explicit KMMailListViewDelegate(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QStyledItemDelegate::paint().
     */
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QStyledItemDelegate::sizeHint().
     */
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Calculate the item height.
     * \param pixelSize Font pixel size.
     * \return The item height via given the font pixel size.
     */
    static int itemHeight(int pixelSize);

signals:

public slots:

private slots:
    void retranslate();

private:
    QString m_emptyTitle;
};

#endif // KMMAILLISTVIEWDELEGATE_H
