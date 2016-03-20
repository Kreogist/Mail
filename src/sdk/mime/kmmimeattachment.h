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

#ifndef KMMIMEATTACHMENT_H
#define KMMIMEATTACHMENT_H

#include "kmmimefile.h"

/*!
 * \brief The KMMimeAttachment class provides the ability to describe the E-mail
 * attachment in mime format.
 */
class KMMimeAttachment : public KMMimeFile
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMimeAttachment object.
     * \param parent The parent object pointer.
     */
    KMMimeAttachment(QObject *parent = 0) :
        KMMimeFile(parent)
    {
        //Set properties.
        setAppendixHeader("Content-disposition: attachment\r\n");
    }
};

#endif // KMMIMEATTACHMENT_H
