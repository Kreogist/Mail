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
#ifndef KNMAILTREEVIEWBASE_H
#define KNMAILTREEVIEWBASE_H

#include <QTreeView>

class QTimeLine;
/*!
 * \brief The KNMailTreeViewBase class provides the tree view of the mail model.
 */
class KNMailTreeViewBase : public QTreeView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailTreeViewBase widget.
     * \param parent The parent widget.
     */
    explicit KNMailTreeViewBase(QWidget *parent = 0);


signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QTreeView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the custom vertical scroll bar widget.
     * \return The scroll bar widget.
     */
    QScrollBar *customScrollBar();

private slots:
    void onActionRangeChanged(int min, int max);
    void onActionValueChanged(int value);

private:
    inline void updateScrollBarGeometry();
    QScrollBar *m_scrollBar;
};

#endif // KNMAILTREEVIEWBASE_H
