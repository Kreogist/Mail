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
#include "kmmimemultipart.h"

KMMimeMultiPart::KMMimeMultiPart(QObject *parent) : KMMimePart(parent)
{

}

KMMimeMultiPart::~KMMimeMultiPart()
{
    //Remove all the sub parts.
    qDeleteAll(m_mimeParts);
}

bool KMMimeMultiPart::addMimePart(KMMimePart *mimePart)
{
    //Check the mime part is empty or not.
    if(mimePart==nullptr)
    {
        //We won't add it.
        return false;
    }
    //Add the content to list.
    m_mimeParts.append(mimePart);
    //Finished.
    return true;
}

QByteArray KMMimeMultiPart::content() const
{
    return QByteArray();
}

bool KMMimeMultiPart::isMultipart() const
{
    return true;
}

KMMimePart *KMMimeMultiPart::part(int i)
{
    return m_mimeParts.at(i);
}

int KMMimeMultiPart::partCount() const
{
    return m_mimeParts.size();
}

void KMMimeMultiPart::setContent(const QByteArray &content)
{
    Q_UNUSED(content);
}
