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

#include "kmquotedprintable.h"

#include "kmmimebase.h"

#include <QDebug>

KMMimeBase::KMMimeBase(QObject *parent) :
    QObject(parent),
    m_content(QByteArray()),
    m_encoding(Mime7Bit)
{
}

QString KMMimeBase::appendixHeader() const
{
    return m_propertyMap.value("appendixHeader", QString());
}

void KMMimeBase::setAppendixHeader(const QString &header)
{
    m_propertyMap.insert("appendixHeader", header);
}

QByteArray KMMimeBase::content() const
{
    return m_content;
}

void KMMimeBase::setContent(const QByteArray &content)
{
    m_content = content;
}

QString KMMimeBase::contentId() const
{
    return m_propertyMap.value("contentId", QString());
}

void KMMimeBase::setContentId(const QString &contentId)
{
    m_propertyMap.insert("contentId", contentId);
}

QString KMMimeBase::contentName() const
{
    return m_propertyMap.value("contentName", QString());
}

void KMMimeBase::setContentName(const QString &contentName)
{
    m_propertyMap.insert("contentName", contentName);
}

QString KMMimeBase::contentType() const
{
    return m_propertyMap.value("contentType", QString());
}

void KMMimeBase::setContentType(const QString &contentType)
{
    m_propertyMap.insert("contentType", contentType);
}

QString KMMimeBase::contentCharset() const
{
    return m_propertyMap.value("contentCharset", QString());
}

void KMMimeBase::setContentCharset(const QString &contentCharset)
{
    m_propertyMap.insert("contentCharset", contentCharset);
}

KMMimeBase::MimeEncoding KMMimeBase::encoding() const
{
    return m_encoding;
}

QString KMMimeBase::toString()
{
    //============Header============
    //------------Content Type------------
    //Prepare the mime data.
    QString mimeData("Content-Type: ");
    mimeData.append(m_propertyMap.value("contentType", QString()));
    //Check content type properties.
    if(m_propertyMap.contains("contentName"))
    {
        //Append name data to mime data.
        mimeData.append("; name=\"").append(contentName()).append("\"");
    }
    //Check charset type properties.
    if(m_propertyMap.contains("contentCharset"))
    {
        //Append charset property to mime data.
        mimeData.append("; charset=").append(contentCharset());
    }
    //Check boundary.
    if(m_propertyMap.contains("contentBoundary"))
    {
        //Append content boundary.
        mimeData.append("; boundary=").append(contentBoundary());
    }
    //Append next line.
    mimeData.append("\r\n");
    //------------Encoding------------
    mimeData.append("Content-Transfer-Encoding: ");
    //Check out encoding type.
    switch (m_encoding)
    {
    case Mime7Bit:
        mimeData.append("7bit\r\n");
        break;
    case Mime8Bit:
        mimeData.append("8bit\r\n");
        break;
    case MimeBase64:
        mimeData.append("base64\r\n");
        break;
    case MimeQuotedPrintable:
        mimeData.append("quoted-printable\r\n");
        break;
    }
    //------------Content ID------------
    if(m_propertyMap.contains("contentId"))
    {
        //Add content id to string text.
        mimeData.append("Content-ID: <").append(contentId()).append(">\r\n");
    }
    //------------Add header data------------
    mimeData.append(appendixHeader()).append("\r\n");

    //============Content============
    switch(m_encoding)
    {
    case Mime7Bit:
        mimeData.append(QString(m_content).toLatin1());
        break;
    case Mime8Bit:
        mimeData.append(m_content);
        break;
    case MimeBase64:
        mimeData.append(formatBase64(m_content.toBase64()));
        break;
    case MimeQuotedPrintable:
        mimeData.append(formatQuotedPrintable(
                            KMQuotedPrintable::encode(m_content)));
        break;
    }
    mimeData.append("\r\n");

    //Complete.
    return mimeData;
}

QString KMMimeBase::formatBase64(const QString &content, int maxLength)
{
    //Generate result cache.
    QString result;
    //Initial the counter.
    int counter=0;
    //Process all content.
    for(int i=0; i<content.size(); ++i)
    {
        //Check counter.
        if(counter > maxLength)
        {
            //To a new line
            result.append("\r\n");
            //Reset counter.
            counter=0;
        }
        //Append the data.
        result.append(content.at(i));
        //Increase counter.
        ++counter;
    }
    //Get the result.
    return result;
}

QString KMMimeBase::formatQuotedPrintable(const QString &content, int maxLength)
{
    //Generate result cache.
    QString result;
    //Initial the counter.
    int counter=0;
    //Process all content.
    for(int i=0; i<content.size(); ++i)
    {
        //Increase counter.
        ++counter;
        //Check whether the content is '\n'
        if (content.at(i)=='\n')
        {
            //Append a new line.
            result.append(content.at(i));
            //Reset the counter.
            counter=0;
            //Continue;
            continue;
        }
        //Check counter.
        if(counter >= maxLength ||
                (content.at(i)=='=' && counter > maxLength - 3))
        {
            //To a new line
            result.append("=\r\n");
            //Reset counter.
            counter=1;
        }
        //Append the data.
        result.append(content.at(i));
    }
    //Get the result.
    return result;
}

void KMMimeBase::setEncoding(const MimeEncoding &encoding)
{
    m_encoding = encoding;
}

void KMMimeBase::insertProperty(const QString &valueName, const QString &value)
{
    m_propertyMap.insert(valueName, value);
}

QString KMMimeBase::getProperty(const QString &valueName,
                                const QString &defaultValue)
{
    return m_propertyMap.value(valueName, defaultValue);
}

bool KMMimeBase::containsProperty(const QString &valueName)
{
    return m_propertyMap.contains(valueName);
}

void KMMimeBase::setContentBoundary(const QString &contentBoundary)
{
    m_propertyMap.insert("contentBoundary", contentBoundary);
}

QString KMMimeBase::contentBoundary() const
{
    return m_propertyMap.value("contentBoundary", QString());
}

