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
#ifndef KNMAILFOLDERTREEVIEW_H
#define KNMAILFOLDERTREEVIEW_H

#include "knmailtreeviewbase.h"

/*!
 * \brief The KNMailFolderTreeView class provides the folder tree view for the
 * folder. The tree view could be used to show the tree view of a mail model.
 */
class KNMailFolderTreeView : public KNMailTreeViewBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailFolderTreeView widget.
     * \param parent The parent widget pointer.
     */
    explicit KNMailFolderTreeView(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMailTreeViewBase::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailTreeViewBase::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionThemeChange();
    void onActionMouseInOut(int opacity);

private:
    inline void startScrollAnime(int targetAlpha);
    QTimeLine *m_mouseAnime;
};

#endif // KNMAILFOLDERTREEVIEW_H
