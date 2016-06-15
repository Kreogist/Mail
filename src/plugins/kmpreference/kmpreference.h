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
#ifndef KMPREFERENCE_H
#define KMPREFERENCE_H

#include "kmpreferencebase.h"
/*!
 * \brief The KMPreference class is to help users set preferences of the Mail
 * application.
 */
class KMPreference : public KMPreferenceBase
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KMPreference widget with the parent widget.
     * \param parent The parent widget pointer.
     */
    explicit KMPreference(QWidget *parent = 0);

signals:

public slots:
};

#endif // KMPREFERENCE_H
