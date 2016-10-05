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
#include "knmailutil.h"

#include "knmimepart.h"

using namespace MailUtil;

#define LineContent (77)

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

QString KNMimePart::toString()
{
    //Prepare the cache.
    QString mimeCache, encoding, encodedContent;
    //Translate all the header to cache.
    headerToString(mimeCache, encoding);
    //Add one empty line.
    mimeCache.append("\r\n");
    //Translate all the content to the encoding.
    if(encoding.isEmpty())
    {
        //Use UTF-8.
        encoding="UTF-8";
    }
    else
    {
        //Use the encoding.
        encoding=encoding.toUpper();
    }
    //Get the encoded content.
    KNMailUtil::encodeContent(m_content,
                              encoding,
                              encodedContent);
    //Check encoded content data.
    while(encodedContent.size()>LineContent)
    {
        //Append the first 76 letter to content.
        mimeCache.append(encodedContent.left(LineContent) + "\r\n");
        //Remove content.
        encodedContent.remove(0, LineContent);
    }
    //Append the content to the last line.
    mimeCache.append(encodedContent + "\r\n");
    //Give back the mime cache.
    return mimeCache;
}

void KNMimePart::headerToString(QString &headerCache, QString &encoding)
{
    //Do the loop.
    QList<QString> propertyList=m_propertyList.keys();
    //Loop and set all the content to the header cache.
    for(int i=0; i<propertyList.size(); ++i)
    {
        const QString &currentKey=propertyList.at(i);
        //Append the data to cache.
        headerCache.append(currentKey + ": " + m_propertyList.value(currentKey)
                           +"\r\n");
        //Check key data.
        if(currentKey.toLower()=="content-transfer-encoding")
        {
            //Save the encoding.
            encoding=m_propertyList.value(currentKey);
        }
    }
}
