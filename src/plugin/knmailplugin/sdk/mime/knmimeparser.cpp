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

#include "knmimeparser.h"

#include <QDebug>

KNMimePart *KNMimeParser::parseMime(const QList<QByteArray> &contents)
{
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
            //Get the current line.
            QByteArray line=contents.at(lineIndex);
            //When we find out an empty line, our the mission should be done.
            if(line.size()==1)
            {
                //The break is an empty line.
                break;
            }
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
            int colonPosition=line.indexOf(": ");
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
    qDebug()<<"Has content-type check"<<headerList.contains("content-type");
    if(headerList.contains("content-type"))
    {
        //Get the content type data.
        const QString &contentType=headerList.value("content-type");
        qDebug()<<"Content-type is"<<contentType;
        //If the content type contains "multipart", then it means it is a
        //multipart data.
        if(contentType.contains("multipart"))
        {
            //---------------Get boundary---------------
            //Prepare the boundary data.
            QByteArray boundary;
            //Parse the boundary from the data.
            {
                //Get the content property value.
                QStringList contentTypeData=contentType.split(';');
                //Find the 'boundary'
                for(QString i : contentTypeData)
                {
                    //Find mark '='.
                    int equalPosition=i.indexOf('=');
                    //Check equal position.
                    if(equalPosition==-1)
                    {
                        //Check next part.
                        continue;
                    }
                    //Get the field name of the part, if it is boundary, get the
                    //data.
                    if(i.left(equalPosition).simplified() == "boundary")
                    {
                        //Save the boundary value.
                        boundary=i.mid(equalPosition+1).toLatin1().simplified();
                        //Stop running.
                        break;
                    }
                }
            }
            //Check the boundary is get or not.
            if(boundary.isEmpty())
            {
                //There's no way that a multipart mime doesn't has a boundary
                //when it is mentioned.
                return nullptr;
            }
            qDebug()<<"boundary is "<<boundary;
            //---------------Tweak boundary format---------------
            //Check the boundary has the quote mark beside it.
            if(boundary.at(0)=='\"' && boundary.at(boundary.size()-1)=='\"'
                    && boundary.size()>2)
            {
                //It contains quote mark on both side, remove them.
                boundary=boundary.mid(1, boundary.size()-2);
            }
            //Append '--' before the boundary.
            boundary.prepend("--");
            //---------------Parse Multipart Mime---------------
            //Construct a multipart content part object.
            KNMimeMultiPart *mimeContent=new KNMimeMultiPart();
            //Set the properties.
            mimeContent->setMimeHeaderList(headerList);
            //Read through all the contenty left.
            for(int i=0, lastBoundary=-1; i<contents.size(); ++i)
            {
                //Check the line is boundary or not.
                if(contents.at(i).left(boundary.size())==boundary)
                {
                    //Check last boundary position.
                    if(lastBoundary==-1)
                    {
                        //This is the first boundary.
                        lastBoundary=i;
                        //Move the next line.
                        continue;
                    }
                    //Check the current position.
                    if(i>lastBoundary)
                    {
                        KNMimePart *resultPart=parseMime(contents.mid(lastBoundary+1,
                                                                      i-lastBoundary-2));
                        //Append the mime part to the mime content.
                        mimeContent->append(resultPart);
                    }
                    //Check whether the boundary is the last line of content.
                    //The last boundary will add "--"  at the end of the
                    //boundary.
                    if(boundary.size()<contents.at(i).simplified().size())
                    {
                        //Mission complete.
                        break;
                    }
                    //Save the current position as the last position.
                    lastBoundary=i;
                }
            }
            //Give back the mime content.
            return mimeContent;
        }
        //For those who doesn't mention the type, treat as single mime part.
    }
    //All the left parts will be treat as a single mime part.
    KNMimePart *mimeContent=new KNMimePart();
    //Give the header to mime content;
    mimeContent->setMimeHeaderList(headerList);
    //Combine all the content data together.
    QByteArray dataContainer;
    //Append all the data together.
    for(int i=lineIndex; i<contents.size(); ++i)
    {
        //Get the content line.
        QString contentLine=contents.at(i);
        //Check the contents data.
        if(i!=contents.size()-1 &&
                contentLine.size()>1 &&
                contentLine.at(contentLine.size()-2)=='=')
        {
            //Remove last equal.
            contentLine.remove(contentLine.size()-2, 2);
        }
        //Save the data to container.
        dataContainer.append(contentLine);
    }
    //Give the data container to the mime part.
    mimeContent->setBody(dataContainer);
    //Give back the mime content.
    return mimeContent;
}

KNMimePart *KNMimeParser::parseMime(const QByteArray &content)
{
    //Split the content.
    QList<QByteArray> contents=content.split('\n');
    //Give back the parse result.
    return parseMime(contents);
}

KNMimePart *KNMimeParser::parseMime(const QString &filePath)
{
    //Build the cache.
    QByteArray fileContent;
    {
        //Read the file.
        QFile mailFile(filePath);
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
    return parseMime(fileContent);
}
