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

#ifndef KMUTIL_H
#define KMUTIL_H

#include <QColor>
#include <QString>

/*!
 * \brief The KMUtil class cannot be construct. It will only used to provides
 * several public static functions which might be used globally.
 */
class KMUtil
{
public:
    /*!
     * \brief Parse a color from a string. The format of the string can be: \n
     *  * RGB
     *  * #RGB
     *  * RRGGBB
     *  * #RRGGBB
     *  * RRGGBBAA
     *  * #RRGGBBAA
     * All the R, G, B and A above can be a hex number range 0-F.
     * \param data The color data string.
     * \return A QColor parsed from the string.
     */
    static QColor parseColor(const QString &data);

    /*!
     * \brief This function will try to make the path valid.
     * \param path The prefer path.
     * \return The simplified path of the prefer path.
     */
    static QString ensurePathValid(const QString &path);

    /*!
     * \brief Simplified a file path. Sometimes the file path may be like:
     * /usr/bin//a, or /usr/bin/../bin/a. Both these path will be simplified to
     * /usr/bin/a.
     * \param path The file path.
     * \return The simplified file path.
     */
    static QString simplifiedPath(const QString &path);

private:
    KMUtil();
    KMUtil(const KMUtil &);
};

#endif // KMUTIL_H
