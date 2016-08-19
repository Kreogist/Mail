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

#ifndef KNMAILPARSER_H
#define KNMAILPARSER_H

#include <QByteArray>
#include <QString>

class KNMimePart;
/*!
 * \brief The KNMailParser class provides the function which could translate a
 * content into a mime format data. This class cannot be constructed.
 */
class KNMailParser
{
public:
    static KNMimePart *parseMail(const QByteArray &content);
    static KNMimePart *parseMail(const QString &mailPath);

private:
    KNMailParser();
    KNMailParser(const KNMailParser &);
    KNMailParser(KNMailParser &&);
};

#endif // KNMAILPARSER_H
