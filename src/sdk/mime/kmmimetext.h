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

#ifndef KMMIMETEXT_H
#define KMMIMETEXT_H

#include "kmmimebase.h"

/*!
 * \brief The KMMimeText class provides the mime format description of an text
 * part of the whole mime format.
 */
class KMMimeText : public KMMimeBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMimeText object.
     * \param parent The parent object.
     */
    KMMimeText(QObject *parent = 0) :
        KMMimeBase(parent)
    {
        //Set properties.
        setContentType("text/plain");
        setContentCharset("utf-8");
        setEncoding(Mime8Bit);
    }
};

#endif // KMMIMETEXT_H
