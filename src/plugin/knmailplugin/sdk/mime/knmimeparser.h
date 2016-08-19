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

#ifndef KNMIMEPARSER_H
#define KNMIMEPARSER_H

#include <QByteArray>
#include <QString>

class KNMimePart;
/*!
 * \brief The KNMailParser class provides the function which could translate a
 * content into a mime format data. This class cannot be constructed.
 */
class KNMimeParser
{
public:
    /*!
     * \brief Parse MIME from raw byte content lines.
     * \param contents The content list. Each item is one line of content.
     * \return The parse mime part pointer.
     */
    static KNMimePart *parseMime(const QList<QByteArray> &contents);

    /*!
     * \brief Parse MIME from raw byte content.
     * \param content The content array.
     * \return The parse mime part pointer.
     */
    static KNMimePart *parseMime(const QByteArray &content);

    /*!
     * \brief Parse MIME from a specific file.
     * \param filePath The mime file path.
     * \return The parse mime part pointer.
     */
    static KNMimePart *parseMime(const QString &filePath);

private:
    KNMimeParser();
    KNMimeParser(const KNMimeParser &);
    KNMimeParser(KNMimeParser &&);
};

#endif // KNMIMEPARSER_H
