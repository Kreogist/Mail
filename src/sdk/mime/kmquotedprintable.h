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

#ifndef KMQUOTEDPRINTABLE_H
#define KMQUOTEDPRINTABLE_H

#include <QString>

/*!
 * \brief The KMQuotedPrintable class provides the encode and decode function of
 * Quote Printable encoding.
 */
class KMQuotedPrintable
{
public:
    /*!
     * \brief Encode an byte array data to readable text.
     * \param data Raw data.
     * \return Readable text string.
     */
    static QString encode(const QByteArray &data);

    /*!
     * \brief Decode quoted readable data to raw data.
     * \param text Encoded quoted readable text.
     * \return Raw data.
     */
    static QByteArray decode(const QString &text);
};

#endif // KMQUOTEDPRINTABLE_H
