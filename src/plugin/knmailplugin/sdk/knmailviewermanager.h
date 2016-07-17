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
#ifndef KNMAILVIEWERMANAGER_H
#define KNMAILVIEWERMANAGER_H

#include <QMap>

#include <QObject>

class KNMailViewerBase;
/*!
 * \brief The KNMailViewerManager class it will manage all the popup mail viewer
 * widget. This manager will record all the file path of the popup viewer. When
 * user is asking for a new popup viewer, it will check the path first, if the
 * mail viewer is already popup, then it will show the viewer directly instead
 * of create a new one.
 */
class KNMailViewerManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailViewerManager object with given parent.
     * \param parent The parent object.
     */
    explicit KNMailViewerManager(QObject *parent = 0);

signals:

public slots:

private:
    QMap<QString, KNMailViewerBase *> m_viewerMap;
};

#endif // KNMAILVIEWERMANAGER_H
