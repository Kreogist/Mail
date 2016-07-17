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
#ifndef KNMAILFOLDERVIEWERBASE_H
#define KNMAILFOLDERVIEWERBASE_H

#include <QWidget>

class KNMailModel;
class KNMailViewerBase;
/*!
 * \brief The KNMailFolderViewerBase class provides the ports, signals and slots
 * which will be used in a folder viewer widget. To realize a new folder viewer,
 * implement this class and realize all the virtual function.\n
 * It should contains a tree view to show all the content from a mail folder
 * model.
 */
class KNMailFolderViewerBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailFolderViewerBase widget with given parent.
     * \param parent The parent widget pointer.
     */
    KNMailFolderViewerBase(QWidget *parent = 0) : QWidget(parent){}

signals:

public slots:
    /*!
     * \brief Set the display folder model.
     * \param folderModel The folder model.
     */
    virtual void setFolderModel(KNMailModel *folderModel)=0;

    /*!
     * \brief Set the signel mail viewer.
     * \param viewer The mail viewer widget.
     */
    virtual void setViewer(KNMailViewerBase *viewer)=0;
};

#endif // KNMAILFOLDERVIEWERBASE_H
