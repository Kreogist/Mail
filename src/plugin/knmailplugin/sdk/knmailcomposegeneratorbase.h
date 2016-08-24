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

#ifndef KNMAILCOMPOSEGENERATORBASE_H
#define KNMAILCOMPOSEGENERATORBASE_H

class KNMailComposeBase;
/*!
 * \brief The KNMailComposeGeneratorBase class provides the port for the
 * composer generator. It should be provided by each compose widget.
 */
class KNMailComposeGeneratorBase
{
public:
    /*!
     * \brief Construct a KNMailComposeGeneratorBase widget.
     */
    KNMailComposeGeneratorBase(){}

    /*!
     * \brief Generate a composer window.
     * \return The composer widget pointer.
     */
    virtual KNMailComposeBase *generateComposer()=0;
};

#endif // KNMAILCOMPOSEGENERATORBASE_H
