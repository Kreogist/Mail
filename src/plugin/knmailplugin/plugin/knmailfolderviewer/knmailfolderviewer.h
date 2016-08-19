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

#include <QModelIndex>

#include "knmailfolderviewerbase.h"

class QTimeLine;
class KNClickableLabel;
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

    /*!
     * \brief Reimplemented from KNMailFolderViewerBase::hideViewer().
     */
    void hideViewer() Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMailFolderViewerBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onSelectionChange(const QModelIndex &current);

private:
    inline void updateViewerSize();
    inline void updateShadowDarkness(const qreal &alpha);
    inline void startAnimeViewer(int endFrame);
    KNMailFolderViewerTitle *m_title;
    KNMailFolderTreeView *m_folderView;
    KNClickableLabel *m_shadowLayer;
    KNMailViewerBase *m_viewer;
    QTimeLine *m_viewerAnime;
    bool m_isAnimeShown;
};

#endif // KNMAILFOLDERVIEWER_H
