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

#include "knmimemultipart.h"

#include <QDebug>

KNMimeMultiPart::KNMimeMultiPart() :
    KNMimePart()
{

}

KNMimeMultiPart::~KNMimeMultiPart()
{
    //Delete all the mime content in the mime parts.
    for(auto i : m_mimeParts)
    {
        //Remove the part.
        delete i;
    }
}

void KNMimeMultiPart::append(KNMimePart *mimePart)
{
    m_mimeParts.append(mimePart);
}

bool KNMimeMultiPart::isMultiPart() const
{
    return true;
}

KNMimePart *KNMimeMultiPart::mimePart(int i) const
{
    return m_mimeParts.at(i);
}

int KNMimeMultiPart::mimePartCount() const
{
    return m_mimeParts.size();
}

QList<KNMimePart *> KNMimeMultiPart::contentList()
{
    //Generate the item list.
    QList<KNMimePart *> itemList;
    //Search through all the sub item.
    for(auto i : m_mimeParts)
    {
        if(i!=nullptr)
        {
            //Add sub item list to current.
            itemList.append(i->contentList());
        }
    }
    qDebug()<<"List size:"<<itemList.size();
    //Return the current item list.
    return itemList;
}

QByteArray KNMimeMultiPart::body() const
{
    return QByteArray();
}

void KNMimeMultiPart::setBody(const QByteArray &body)
{
    Q_UNUSED(body)
}
