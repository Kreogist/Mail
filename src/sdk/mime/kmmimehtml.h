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

#ifndef KMMIMEHTML_H
#define KMMIMEHTML_H

#include "kmmimetext.h"

/*!
 * \brief The KMMimeText class provides the mime format description of an html
 * format document in the whole mime format.\n
 * Html text is treated as a special format text here.
 */
class KMMimeHtml : public KMMimeText
{
    Q_OBJECT
public:
    /*!
     * \brief Construct an KMMimeHtml object.
     * \param parent The parent object.
     */
    KMMimeHtml(QObject *parent = 0) :
        KMMimeText(parent)
    {
        //Set properties.
        setContentType("text/html");
    }
};

#endif // KMMIMEHTML_H
