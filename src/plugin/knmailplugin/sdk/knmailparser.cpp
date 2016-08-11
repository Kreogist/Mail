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
#include <QFile>

#include "knmimepart.h"
#include "knmimemultipart.h"

#include "knmailparser.h"


KNMimePart *KNMailParser::parseMail(const QByteArray &content)
{
    //Split the content.
    QList<QByteArray> contents=content.split('\n');
    //Check content lines.
    if(contents.isEmpty())
    {
        //Cannot be parsed for an empty list.
        return nullptr;
    }
    //Prepare for the loops.
    int lineIndex=0;
    //Read the header parts of the content.
    QMap<QString, QString> headerList;
    {
        //Prepare the header field and value.
        QString headerField, headerValue;
        //Check all the header parts.
        while(lineIndex < contents.size())
        {
            //When we find out an empty line, our the mission should be done.
            if(contents.at(lineIndex).isEmpty())
            {
                //The break is an empty line.
                break;
            }
            //Get the current line.
            const QByteArray &line=contents.at(lineIndex);
            //Check the data is start with space/tab or not.
            if(line.startsWith(' ') || line.startsWith('\t'))
            {
                //This line's value should be append to the previous header
                //value.
                headerValue.append(line);
                //Move to next line.
                ++lineIndex;
                continue;
            }
            //Check the field data.
            if(!headerField.isEmpty())
            {
                //Add previous data to the header field.
                headerList.insert(headerField, headerValue);
                //Reset the header field and value.
                headerField=QString();
                headerValue=QString();
            }
            //Split the current line.
            int colonPosition=line.indexOf(': ');
            //Check the line contains colon or not. (-1 for not found)
            if(colonPosition<0)
            {
                //Abandon the line.
                ++lineIndex;
                continue;
            }
            //Get the field and value.
            headerField=line.left(colonPosition).toLower();
            headerValue=line.mid(colonPosition+2);
            //Move to next line.
            ++lineIndex;
        }
        //Check field data.
        if(!headerField.isEmpty())
        {
            //Insert the last field and value data into the map.
            headerList.insert(headerField, headerValue);
        }
    }
    //Check the property data field, it should contains a property named
    //"Content-type".
    if(headerList.contains("content-type"))
    {
        //Get the content type data.
        const QString &contentType=headerList.value("content-type");
        //If the content type contains "multipart", then it means it is a
        //multipart data.
        if(contentType.contains("multipart"))
        {
            //Construct a multipart object.
            KNMimeMultiPart *mimeContent=nullptr;
            //Prepare the boundary data.
        }
    }
    //All the left parts will be treat as a single mime part.
    KNMimePart *mimeContent=new KNMimePart();
    //Give ;
    return mimeContent;
}

KNMimePart *KNMailParser::parseMail(const QString &mailPath)
{
    //Build the cache.
    QByteArray fileContent;
    {
        //Read the file.
        QFile mailFile(mailPath);
        if(!mailFile.open(QIODevice::ReadOnly))
        {
            //Mail file cannot be open via read only mode.
            return nullptr;
        }
        //Read the content.
        fileContent=mailFile.readAll();
        //Close the file.
        mailFile.close();
    }
    //Give back the parse result.
    return parseMail(fileContent);
}
