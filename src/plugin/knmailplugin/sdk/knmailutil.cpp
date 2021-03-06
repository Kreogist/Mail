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
#include <QTextCodec>

#include "knquotedprintable.h"

#include "knmailutil.h"

#include <QDebug>

#define EncodingStartMark "=?"
#define EncodingEndMark "?="

QString KNMailUtil::fromUtf7(const QByteArray &data)
{
    //Prepare the return value cache.
    QString decodedText;
    /*
     * All the UTF-7 format should be like:
     *      ABCDEF &i6KWBQ-
     *      [AscII][Encoded]
     * All the encoded part will be around with & and -, the ASC II part won't
     * need to decode. All raw data of the encoded part are encoded in UTF-16BE.
     */
    //Now the text is from its original codec, which is UTF-16BE.
    QTextCodec *codec=QTextCodec::codecForName("UTF-16BE");
    //Search for char '&'
    int andPosition=-1, hyphenPosition=-1;
    for(int i=0; i<data.length(); ++i)
    {
        //Check current char.
        //Find start.
        if(data.at(i)=='&')
        {
            //Update the & position.
            andPosition=i;
            continue;
        }
        //Find end.
        if(data.at(i)=='-' && andPosition!=-1)
        {
            //Add all the previous data to the decoded text.
            int previousStart=hyphenPosition+1,
                    previousLength=andPosition-previousStart;
            //When the previous length is more than 0, append data to decoded
            //text.
            if(previousLength>0)
            {
                //Append data.
                decodedText.append(data.mid(previousStart, previousLength));
            }
            //Set the new hyphen position.
            hyphenPosition=i;
            //Decode the part.
            decodedText.append(codec->toUnicode(
                                   QByteArray::fromBase64(data.mid(
                                       andPosition+1,
                                       hyphenPosition-andPosition-1))));
        }
    }
    //Append all the left data to the decoded text.
    decodedText.append(data.mid(hyphenPosition+1));
    //Give back the decoded text.
    return decodedText;
}

QString KNMailUtil::parseEncoding(QString data)
{
    //Prepare the caches.
    QString parsedText, encodedPart;
    //Find the =? and ?=
    int dataStart=data.indexOf(EncodingStartMark), dataEnd=-1;
    //Loop until data start.
    while(dataStart>-1)
    {
        //Find the data end.
        dataEnd=data.indexOf(EncodingEndMark);
        //Check whether the data end is valid.
        while(dataEnd>-1 &&
              //If the "?=" is not the last one, or
              (dataEnd!=data.length()-2) &&
                //It is not followed by a space or a quote mark.
                (data.at(dataEnd+2)!=' ') &&
                (data.at(dataEnd+2)!='"'))
        {
            //Find the next data end.
            dataEnd=data.indexOf(EncodingEndMark, dataEnd+1);
        }
        //Check the end of the data end.
        if(dataEnd>-1)
        {
            //Pick out the data.
            encodedPart=data.mid(dataStart, dataEnd-dataStart+2);
            //Get the previous data.
            parsedText.append(data.left(dataStart));
            //Get the encoding part.
            parsedText.append(parseEncodingPart(encodedPart));
            //Remove the data.
            data.remove(0, dataEnd+2);
            //Check data is start with a spacing or not, if so, remove the
            //spacing.
            if(!data.isEmpty() && (data.at(0)==' '))
            {
                //Remove the splitter spacing.
                data.remove(0, 1);
            }
        }
        //Update the data start postion.
        dataStart=data.indexOf(EncodingStartMark);
    }
    //Check whether data is empty or not.
    if(!data.isEmpty())
    {
        //Add the left part to parsed text.
        parsedText.append(data);
    }
    //Give the parsed result back.
    return parsedText;
}

QString KNMailUtil::parseMailAddress(const QString &rawData,
                                     QString &addressName)
{
    //Search the content of '<' in the raw data.
    int startPosition=rawData.indexOf('<');
    //Check thte start position.
    if(startPosition==-1)
    {
        //No result find.
        return rawData.trimmed();
    }
    //Find the end position for '>'.
    int endPosition=rawData.indexOf('>');
    //Check the end position.
    if(endPosition==-1)
    {
        //No result find.
        return QString();
    }
    //Parse the address name.
    addressName=KNMailUtil::parseEncoding(
                rawData.left(startPosition).simplified());
    //Give back the email address.
    return rawData.mid(startPosition+1,
                       endPosition-startPosition-1).trimmed();
}

void KNMailUtil::parseContent(const QByteArray &content,
                              const QString &encoding,
                              QByteArray &decodedContent)
{
    //Check the encoding.
    if(encoding=="QUOTED-PRINTABLE")
    {
        //Give back the raw data parsed with quoted printable.
        decodedContent=KNQuotedPrintable::decode(content);
        //Mission complete.
        return;
    }
    if(encoding=="BASE64")
    {
        //Give back the BASE64 decoding result.
        decodedContent=QByteArray::fromBase64(content);
        //Mission complete.
        return;
    }
    //For the left things:
    // - 7bit
    // - 8bit
    //Treat them as raw data.
    decodedContent=content;
}

void KNMailUtil::encodeContent(const QByteArray &content,
                               const QString &encoding,
                               QString &encodedContent)
{
    //Check encoding.
    if(encoding=="QUOTED-PRINTABLE")
    {
        //Encode the data.
        encodedContent=KNQuotedPrintable::encode(content);
        //Mission complete.
        return;
    }
    if(encoding=="BASE64")
    {
        //Give back the BASE64 encoding result.
        encodedContent=content.toBase64();
        //Mission complete.
        return;
    }
    //For the left things:
    // - 7bit
    // - 8bit
    //Treat them as raw data.
    encodedContent=content;
}

QString KNMailUtil::parseEncodingPart(const QString &data)
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
                            secondQuestionMark-firstQuestionMark-1).toUpper();
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
                        KNQuotedPrintable::decode(
                            content.mid(secondQuestionMark+1)));
        }
        return content;
    }
    //Or else, we don't need to parse.
    return data;
}
