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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNPLATFORMEXTRAS_H
#define KNPLATFORMEXTRAS_H

#include <QObject>

/*!
 * \brief The KNPlatformExtras class provides some independent features on
 * different operating systems. This is only an abstract platform resources, on
 * some platform it won't be used.
 */
class KNPlatformExtras : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNPlatformExtras object.
     * \param parent The parent object.
     */
    KNPlatformExtras(QObject *parent = 0) : QObject(parent){}

signals:

public slots:
};

#endif // KNPLATFORMEXTRAS_H
