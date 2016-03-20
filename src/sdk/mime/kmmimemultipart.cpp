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
#include <QCryptographicHash>
#include <QTime>

#include "kmmimemultipart.h"

QString KMMimeMultiPart::m_multiTypeName[KMMimeMultiPart::MultiPartTypeCount] =
{
    "multipart/mixed",         //    Mixed
    "multipart/digest",        //    Digest
    "multipart/alternative",   //    Alternative
    "multipart/related",       //    Related
    "multipart/report",        //    Report
    "multipart/signed",        //    Signed
    "multipart/encrypted"      //    Encrypted
};

KMMimeMultiPart::KMMimeMultiPart(QObject *parent) :
    KMMimeBase(parent),
    m_multiType(Related)
{
    //Set properties.
    setContentType(m_multiTypeName[m_multiType]);
    setEncoding(Mime8Bit);
    //Set the boundary of the mime part.
    setContentBoundary(QCryptographicHash::hash(
                           QByteArray().append(qrand()),
                           QCryptographicHash::Md5).toHex());
}

void KMMimeMultiPart::addPart(KMMimeBase *part)
{
    //Change part relationship.
    part->setParent(this);
    //Add part to list.
    m_parts.append(part);
}

QList<KMMimeBase *> KMMimeMultiPart::parts() const
{
    return m_parts;
}

KMMimeMultiPart::MultiPartType KMMimeMultiPart::multiType() const
{
    return m_multiType;
}

QString KMMimeMultiPart::toString()
{
    //Prepare the multi content.
    QByteArray multiContent;
    //Add all content data to multi content.
    for(auto i=m_parts.begin(); i!=m_parts.end(); ++i)
    {
        //Append data to multi content.
        //--Append content boundary--
        multiContent.append("--").append(contentBoundary()).append("\r\n")
        //--Append mime part data--
                    .append((*i)->toString()).append("\r\n");
    }
    //--Append content boundary end--
    multiContent.append("--").append(contentBoundary()).append("--\r\n");
    //Set the multi content as content.
    setContent(multiContent);
    //Get the mime data.
    return KMMimeBase::toString();
}

void KMMimeMultiPart::setMultiType(const MultiPartType &multiType)
{
    //Save the multi part.
    m_multiType = multiType;
    //Reset the content type.
    setContentType(m_multiTypeName[m_multiType]);
}

