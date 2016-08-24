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

#include "knmimepart.h"

KNMimePart::KNMimePart()
{

}

KNMimePart::~KNMimePart()
{
}

QMap<QString, QString> KNMimePart::headerList() const
{
    return m_propertyList;
}

QString KNMimePart::mimeHeader(const QString &field) const
{
    return m_propertyList.value(field, QString());
}

void KNMimePart::setMimeHeaderList(const QMap<QString, QString> &propertyList)
{
    m_propertyList = propertyList;
}

void KNMimePart::setMimeHeader(const QString &field, const QString &value)
{
    m_propertyList.insert(field, value);
}

void KNMimePart::setBody(const QByteArray &body)
{
    m_content=body;
}

QByteArray KNMimePart::body() const
{
    return m_content;
}

bool KNMimePart::hasMimeHeader(const QString &field) const
{
    return m_propertyList.contains(field);
}

bool KNMimePart::isMultiPart() const
{
    return false;
}
