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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QTextCodec>

#include "kmquotedprintable.h"

#include "kmmailbody.h"

#include <QDebug>

KMMailBody::KMMailBody(QObject *parent) :
    QObject(parent),
    m_contentPart(nullptr),
    m_encoding(KMMimeBase::Mime8Bit)
{
}

QString KMMailBody::element(int elementIndex) const
{
    Q_ASSERT(elementIndex>-1 && elementIndex<MailElementCount);
    //Give back the element.
    return m_element[elementIndex];
}

void KMMailBody::setElement(int elementIndex, const QString &elementData)
{
    Q_ASSERT(elementIndex>-1 && elementIndex<MailElementCount);
    //Save the element.
    m_element[elementIndex]=elementData;
}

void KMMailBody::contactInfo(int contactIndex,
                             QStringList &mailAddress,
                             QStringList &name)
{
    Q_ASSERT(contactIndex>-1 && contactIndex<MailContactCount);
    //Copy the mail address and name array.
    mailAddress=m_contact[contactIndex];
    name=m_contactName[contactIndex];
}

void KMMailBody::setContactInfo(int contactIndex,
                                const QStringList &mailAddress,
                                const QStringList &name)
{
    Q_ASSERT(contactIndex>-1 && contactIndex<MailContactCount &&
             mailAddress.size()==name.size());
    //Save the mail address and name array.
    m_contact[contactIndex]=mailAddress;
    m_contactName[contactIndex]=name;
}

QString KMMailBody::toString()
{
    //Generate the mime data cache.
    QString mimeData("From");
    //---------Mime Header---------
    //Check out the sender name is empty or not.
    if(!m_element[ElementSenderName].isEmpty())
    {
        //Append sender name.
        appendEncoding(mimeData, m_element[ElementSenderName]);
    }
    //Append sender E-mail address.
    mimeData.append(" <").append(m_element[ElementSender]).append(">\r\n");

    //---------Recipients / To---------
    mimeData.append("To:");
    //Append receiver list to mime data.
    appendContact(mimeData, Receiver);
    //---------Recipients / Cc---------
    //Check Carbon Copy contact array size.
    if(!m_contact[CarbonCopy].isEmpty())
    {
        //Append Cc list to mime data.
        appendContact(mimeData, CarbonCopy);
    }

    //---------Subject---------
    mimeData.append("Subject:");
    //Append subject data.
    appendEncoding(mimeData, m_element[ElementSubject]);
    //Append return.
    mimeData.append("\r\n");

    //---------Basic properties---------
    mimeData.append("MIME-Version: 1.0\r\n");

    //---------Content---------
    if(m_contentPart!=nullptr)
    {
        //Append content part data to mime data.
        mimeData.append(m_contentPart->toString());
    }
    return mimeData;
}

void KMMailBody::fromString(const QString &rawData)
{
    //Reset the multipart pointer.
    if(m_contentPart!=nullptr)
    {
        //Remove the original content part.
        m_contentPart->deleteLater();
    }
    //Rebuild the content part.
    m_contentPart=new KMMimeMultiPart(this);
    //First, split the raw data into lines.
    QStringList rawDataLines=rawData.split("\r\n");
    //Check all raw data lines.
    for(auto i=rawDataLines.begin(); i!=rawDataLines.end(); ++i)
    {
        //Check whether this line contains a property.
        int colonPos=(*i).indexOf(':');
        //Check position validation
        if(colonPos==-1)
        {
            //Check whether the multi mime part begin.
            if((*i)==("--" + m_contentPart->contentBoundary()))
            {
                //Ok, the following part will be parse by multi mime part.
//                m_contentPart->fromStringList(
//                            rawDataLines.mid(i-rawDataLines.begin()));
                break;
            }
            //To next line.
            continue;
        }
        //Get the property name.
        QString propertyName=(*i).left(colonPos);
        //Check property, we will only read the data what we need.
        if(propertyName=="From")
        {
            //We need to parse the from email information.
            QString fromData=(*i).mid(colonPos+1).simplified(),
                    fromAddress, fromName;
            //Parse from data.
            if(parseContact(fromData, fromAddress, fromName))
            {
                //Save the address and name.
                m_element[ElementSender]=fromAddress;
                m_element[ElementSenderName]=fromName;
            }
            //To Next Line.
            continue;
        }
        else if(propertyName=="Subject")
        {
            //Simply save the subject data.
            m_element[ElementSubject]=(*i).mid(colonPos+1).simplified();
            //Complete.
            continue;
        }
        else if(propertyName=="To")
        {
            //Copy current line.
            QString contactLine=(*i);
            //Read until the next line is not start with \t.
            while(((i+1)!=rawDataLines.end()) && (*(i+1)).startsWith("\t"))
            {
                //Append content.
                contactLine.append(*(i+1));
                //Move i.
                ++i;
            }
            //Remove the first "To:" data.
            QStringList contactList=contactLine.mid(colonPos+1).split(", "),
                        contactNameList, contactAddressList;
            //Parse the contact list item.
            QString contactName, contactAddress;
            //Parse all information.
            for(auto i=contactList.begin(); i!=contactList.end(); ++i)
            {
                //Parse all the items.
                if(parseContact((*i).simplified(),
                                contactAddress,
                                contactName))
                {
                    //Add to contact list.
                    contactNameList.append(contactName);
                    contactAddressList.append(contactAddress);
                }
            }
            //Complete.
            continue;
        }
        else if(propertyName=="Content-Type")
        {
            //Parse the content type, if the content type is not multi-part,
            //Parse failed.
            //Copy current line.
            QString contentTypeLine=(*i);
            //Read until the next line is not start with \t.
            while(((i+1)!=rawDataLines.end()) && (*(i+1)).startsWith("\t"))
            {
                //Append content.
                contentTypeLine.append(*(i+1));
                //Move i.
                ++i;
            }
            //Remove the first "Content-Type:" data.
            QStringList contentTypeProperty=
                    contentTypeLine.mid(colonPos+1).split(';');
            //Parse all type.
            for(auto i=contentTypeProperty.begin();
                i!=contentTypeProperty.end();
                ++i)
            {
                //Get the simplified content.
                QString property=(*i).simplified();
                //Check whether it contains =:
                int equalPos=property.indexOf('=');
                //Check equal mark pos valid:
                if(equalPos==-1)
                {
                    //Ignore the line.
                    continue;
                }
                //Get property name.
                QString propertyName=property.left(equalPos),
                        propertyValue=property.mid(equalPos+1);
                //Check whether value contains "" at begin and end.
                if(propertyValue.startsWith('\"') &&
                        propertyValue.endsWith('\"') && propertyValue.size()>2)
                {
                    //Remove the first and last.
                    propertyValue=propertyValue.mid(1, propertyValue.size()-2);
                }
                //Check property name.
                if(propertyName=="boundary")
                {
                    //Set the boundary of multipart.
                    m_contentPart->setContentBoundary(propertyValue);
                }
                else if(propertyName=="charset")
                {
                    //Set the charset.
                    m_contentPart->setContentCharset(propertyValue);
                }
                else if(propertyName=="name")
                {
                    //Set the content name.
                    m_contentPart->setContentName(propertyValue);
                }
            }
        }
        qDebug()<<propertyName;
    }
}

inline void KMMailBody::appendContact(QString &target, int contactIndex)
{
    //Append all the contact array data to target.
    for(int i=0, contactSize=m_contact[contactIndex].size();
        i<contactSize;
        ++i)
    {
        //Check i.
        if(i!=0)
        {
            //Add comma between the contact.
            target.append(',');
        }
        //Check contact name is empty or not.
        if(!m_contactName[contactIndex].at(i).isEmpty())
        {
            //Append the contact name to target.
            appendEncoding(target, m_contactName[contactIndex].at(i));
        }
        //Append the E-mail address.
        target.append(" <").append(m_contact[contactIndex].at(i)).append(">");
    }
    //Append return text.
    target.append("\r\n");
}

inline void KMMailBody::appendEncoding(QString &target, const QString &data)
{
    //Append sender name to mime data via different encoding.
    switch(m_encoding)
    {
    //For Base64 encoding, use Qt embedded function to translate.
    case KMMimeBase::MimeBase64:
        target.append(" =?utf-8?B?")
                .append(data.toUtf8().toBase64())
                .append("?=");
        break;
    //For Quoted Printable encoding, use our own encoding function.
    case KMMimeBase::MimeQuotedPrintable:
        target.append(" =?utf-8?Q?")
                .append(KMQuotedPrintable::encode(data.toUtf8())
                        .replace(' ', '_')
                        .replace(':',"=3A"))
                .append("?=");
        break;
    //For the other types, append the raw data.
    default:
        target.append(" ").append(data);
    }
}

QString KMMailBody::parseEncoding(const QString &data)
{
    //Check if the data start with =? and ends with ?=
    if(data.startsWith("=?") && data.endsWith("?=") && data.size()>4)
    {
        //It need to be parsed.
        //Remove the first and last two chars.
        QString content=data.mid(2, data.size()-4);
        //The format of content should be:
        //  Encoding(utf-8, GBK)?B(Base64)/Q(Quoted Printable)?Data
        //Get the question mark pos.
        int firstQuestionMark=content.indexOf('?'),
                secondQuestionMark=content.indexOf('?', firstQuestionMark+1);
        //Check validation of first question mark.
        if(firstQuestionMark==-1 || secondQuestionMark==-1)
        {
            //Give back raw content.
            return data;
        }
        //Get encoding codec.
        QTextCodec *codec=
                QTextCodec::codecForName(
                    content.left(firstQuestionMark).toUtf8());
        //Get data encoding codec.
        QString encodingCodec=
                content.mid(firstQuestionMark+1,
                            secondQuestionMark-firstQuestionMark-1);
        //Check data encoding type.
        if(encodingCodec=="B")
        {
            return codec->toUnicode(
                        QByteArray::fromBase64(
                            content.mid(secondQuestionMark+1).toUtf8()));
        }
        if(encodingCodec=="Q")
        {
            return codec->toUnicode(
                        KMQuotedPrintable::decode(
                            content.mid(secondQuestionMark+1)));
        }
        return content;
    }
    //Or else, we don't need to parse.
    return data;
}

bool KMMailBody::parseContact(const QString &data,
                              QString &address,
                              QString &name)
{
    //Find the '<' char from the last to first.
    int leftPos=data.lastIndexOf('<');
    //Check the left pos.
    if(leftPos<0)
    {
        //Failed to parse.
        return false;
    }
    //Get the name of the contact.
    name=parseEncoding(data.left(leftPos).simplified());
    //GEt the contact address.
    address=data.mid(leftPos+1, data.size()-leftPos-2);
    //Success.
    return true;
}

KMMimeMultiPart *KMMailBody::contentPart() const
{
    return m_contentPart;
}

void KMMailBody::setContentPart(KMMimeMultiPart *contentPart)
{
    //Save content part pointer.
    m_contentPart = contentPart;
    //Check pointer null.
    if(m_contentPart==nullptr)
    {
        return;
    }
    //Reset the parent pointer.
    m_contentPart->setParent(this);
}
