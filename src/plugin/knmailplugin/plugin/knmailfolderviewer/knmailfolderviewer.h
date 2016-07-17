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
#ifndef KNMAILFOLDERVIEWER_H
#define KNMAILFOLDERVIEWER_H

#include "knmailfolderviewerbase.h"

class KNMailModel;
class KNMailFolderViewerTitle;
class KNMailFolderTreeView;
class KNMailViewerBase;
/*!
 * \brief The KNMailFolderViewer class is an official realized which used all
 * SDKs to achieve the basic needs of a folder viewer.
 */
class KNMailFolderViewer : public KNMailFolderViewerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailFolderViewer widget.
     * \param parent The parent widget pointer.
     */
    explicit KNMailFolderViewer(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMailFolderViewerBase::setFolderModel().
     */
    void setFolderModel(KNMailModel *folderModel) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailFolderViewerBase::setViewer().
     */
    void setViewer(KNMailViewerBase *viewer) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMailFolderViewerBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    inline void updateViewerPos();
    KNMailFolderViewerTitle *m_title;
    KNMailFolderTreeView *m_folderView;
    QWidget *m_shadowLayer;
    KNMailViewerBase *m_viewer;
};

#endif // KNMAILFOLDERVIEWER_H
