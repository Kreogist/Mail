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
#include "kmmimepart.h"

KMMimePart::KMMimePart(QObject *parent) :
    QObject(parent)
{

}

QMap<QString, QString> KMMimePart::properties() const
{
    return m_properties;
}

bool KMMimePart::hasMimeProperty(const QString &field) const
{
    return m_properties.contains(field);
}

QString KMMimePart::mimeProperty(const QString &field) const
{
    return m_properties.value(field);
}

void KMMimePart::setMimeProperties(const QMap<QString, QString> &properties)
{
    m_properties = properties;
}

QByteArray KMMimePart::content() const
{
    return m_content;
}

bool KMMimePart::isMultipart() const
{
    return false;
}

void KMMimePart::setMimeProperty(const QString name, const QString &data)
{
    m_properties.insert(name, data);
}

void KMMimePart::setContent(const QByteArray &content)
{
    m_content = content;
}
