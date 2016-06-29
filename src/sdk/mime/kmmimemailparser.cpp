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
#include <QFile>
#include <QTextDocument>
#include <QTimeZone>

#include "kmmimepart.h"
#include "kmmimemultipart.h"
#include "kmmailparseutil.h"

#include "kmmimemailparser.h"

#include <QDebug>

KMMimeMailParser::KMMimeMailParser(QObject *parent) : QObject(parent)
{

}

KMMimePart *KMMimeMailParser::parseContent(const QString &filePath,
                                           QObject *partParent)
{
    //Split all the content into string list.
    QList<QByteArray> contentLines;
    {
        //Read the content from the file path.
        QFile mimeFile(filePath);
        //Try to open the file.
        if(mimeFile.open(QIODevice::ReadOnly))
        {
            //Read the content data.
            contentLines=mimeFile.readAll().split('\n');
            //Close the file.
            mimeFile.close();
        }
    }
    //Give the result back.
    return parseContent(contentLines, partParent);
}

KMMimePart *KMMimeMailParser::parseContent(
        const QList<QByteArray> &contentLines,
        QObject *partParent)
{
    //Check the content lines.
    if(contentLines.isEmpty())
    {
        //No content.
        return nullptr;
    }
    //Get the pointer.
    int currentLine=0;
    //Initial the content map.
    QMap<QString, QString> propertyData;
    //Initial the field and data area.
    QString propertyField, propertyContent;
    //Check the the content.
    while(currentLine < contentLines.size())
    {
        //Check whether the line is an empty line.
        if(contentLines.at(currentLine).isEmpty())
        {
            //Mission complete.
            break;
        }
        //Get the current line.
        const QString &currentData=contentLines.at(currentLine);
        //Check whether the data is start with space.
        if(currentData.startsWith(' ') ||
                currentData.startsWith('\t'))
        {
            //Append data to property.
            propertyContent.append(currentData);
            //Continue to next line.
            ++currentLine;
            //Move next.
            continue;
        }
        //Check the field data.
        if(!propertyField.isEmpty())
        {
            //Add the property to the map.
            propertyData.insert(propertyField, propertyContent);
            //Reset the property field and data.
            propertyField=QString();
            propertyContent=QString();
        }
        //Split the current line.
        int colonPosition=currentData.indexOf(": ");
        //Check the colon position data.
        if(colonPosition<0)
        {
            //Continue to next line.
            ++currentLine;
            //Move next.
            continue;
        }
        //Get the field name.
        propertyField=currentData.left(colonPosition).toLower();
        propertyContent=currentData.mid(colonPosition+2);
        //Continue to next line.
        ++currentLine;
        //Move next.
        continue;
    }
    //Check the field data.
    if(!propertyField.isEmpty())
    {
        //Add the property to the map.
        propertyData.insert(propertyField, propertyContent);
    }
    //Check the property data field, it should contains a property named
    //"Content-type"
    if(propertyData.contains("content-type"))
    {
        //Get the value of content type.
        const QString &contentType=propertyData.value("content-type");
        //If ther content type contains "multipart", then it means it is a
        //multipart data.
        if(contentType.contains("multipart"))
        {
            //Prepare the boundary data;
            QByteArray boundary;
            {
                //Get the content part data, it should contains a property named
                //'boundary'.
                QStringList contentTypeData=contentType.split(';');
                //Find the 'boundary'.
                for(auto i=contentTypeData.begin();
                    i!=contentTypeData.end();
                    ++i)
                {
                    //Check each string.
                    int equalPosition=(*i).indexOf('=');
                    //Check the position.
                    if(equalPosition<0)
                    {
                        //Goto next line.
                        continue;
                    }
                    //Then the equal should be found, get the field name.
                    QString fieldName=(*i).left(equalPosition).simplified();
                    //Check the fieldName.
                    if(fieldName=="boundary")
                    {
                        //Save the boundary data.
                        boundary=(*i).mid(equalPosition+1).toLatin1();
                        //Mission complete.
                        break;
                    }
                }
            }
            //Check the boundary.
            if(boundary.isEmpty())
            {
                //There must be a bug here.
                return nullptr;
            }
            //Check the boundary data.
            if(boundary.at(0)=='\"' && boundary.size()>2)
            {
                //It contains quote mark on both side, remove them.
                boundary=boundary.mid(1, boundary.size()-2);
            }
            boundary="--"+boundary;
            //Construct a content part object.
            KMMimeMultiPart *mimeContent=new KMMimeMultiPart(partParent);
            //Give the property to the content.
            mimeContent->setMimeProperties(propertyData);
            //Split the string list.
            for(int i=currentLine, lastPosition=-1; i<contentLines.size(); ++i)
            {
                //Check the current line.
                if(contentLines.at(i).startsWith(boundary))
                {
                    //Check last position.
                    if(lastPosition==-1)
                    {
                        //Save the current position as last position.
                        lastPosition=i;
                        //Go on to next line.
                        continue;
                    }
                    //Check the current position.
                    if(i>lastPosition)
                    {
                        //Or else, add the data to the mime content.
                        mimeContent->addMimePart(parseContent(contentLines.mid(
                                                     lastPosition+1,
                                                     i-lastPosition-2),
                                                 mimeContent));
                    }
                    //Check whether the content line is exactly the same as the
                    //boundary.
                    if(boundary.size()<contentLines.at(i).size())
                    {
                        //Mission complete.
                        break;
                    }
                    //Save the current position as last position.
                    lastPosition=i;
                }
            }
            //Give back the mail content.
            return mimeContent;
        }
    }
    //It should be treat as a normal data part.
    KMMimePart *mimeContent=new KMMimePart(partParent);
    //Give the property to the content.
    mimeContent->setMimeProperties(propertyData);
    //Combine all the content data together.
    QByteArray dataContainer;
    //Append all the data together.
    for(int i=currentLine; i<contentLines.size(); ++i)
    {
        //Save the data to container.
        dataContainer.append(contentLines.at(i));
        dataContainer.append('\n');
    }
    //Give the data container to the mime part.
    mimeContent->setContent(dataContainer);
    //Give back the mime content.
    return mimeContent;
}

bool KMMimeMailParser::getBriefContent(KMMimePart *mimePart,
                                       QString &content)
{
    //Check the pointer.
    if(mimePart==nullptr)
    {
        //Failed to parse the mime part.
        return false;
    }
    //Check out the content type.
    if(mimePart->hasMimeProperty("content-type"))
    {
        //Check the mime type.
        if(mimePart->isMultipart())
        {
            //Cast to multi part.
            KMMimeMultiPart *multiPart=static_cast<KMMimeMultiPart *>(mimePart);
            //Check each part.
            for(int i=0, partSize=multiPart->partCount(); i<partSize; ++i)
            {
                //Once one part contains text, mission complete.
                if(getBriefContent(multiPart->part(i), content))
                {
                    //Mission complete.
                    return true;
                }
            }
            //Parse each mime part.
            return false;
        }
        //Get the property.
        QString mimeType=mimePart->mimeProperty("content-type").toLower();
        //Check the mime type.
        if(mimeType.contains("plain"))
        {
            //This is the best thing! It is exactly the data.
            content=KMMailParseUtil::parseContent(
                        mimePart->mimeProperty(
                            "content-transfer-encoding").toUpper(),
                        mimePart->content()).left(100);
            //Mission complete when the content is not empty.
            if(!content.isEmpty())
            {
                return true;
            }
        }
        if(mimeType.contains("html"))
        {
            //Use the text document to parse the html.
            QTextDocument document;
            //Set the html data.
            document.setHtml(KMMailParseUtil::parseContent(
                                 mimePart->mimeProperty(
                                     "content-transfer-encoding").toUpper(),
                                 mimePart->content()));
            //Get the html parse result.
            content=document.toPlainText().left(100);
            //Mission complete when the content is not empty.
            if(!content.isEmpty())
            {
                return true;
            }
        }
        //All the others ignore.
        return false;
    }
    return false;
}

QDateTime KMMimeMailParser::getDate(const QString &dateTimeData)
{
    //Construct a date time class.
    QDateTime targetTime;
    //The format should be:
    //  Tue, 27 Jan 2015 16:51:20 +0800 (CST)
    //Check data size.
    if(dateTimeData.size()<30)
    {
        //Give a null date time back.
        return QDateTime();
    }
    //Generate the month letters.
    QStringList monthList;
    monthList << "jan" << "feb" << "mar" << "apr" << "may" << "jun"
              << "jul" << "aug" << "sep" << "oct" << "nov" << "dec";
    //First, get the date data.
    targetTime.setDate(QDate(dateTimeData.mid(12, 4).toInt(),
                             monthList.indexOf(dateTimeData.mid(8, 3).toLower())
                             + 1,
                             dateTimeData.mid(5, 2).toInt()));
    //Then, get the time data.
    targetTime.setTime(QTime(dateTimeData.mid(17, 2).toInt(),
                             dateTimeData.mid(20, 2).toInt(),
                             dateTimeData.mid(23, 2).toInt()));
    //Last the time zone.
    //Actually, I think maybe there will only left the CST?
    targetTime.setTimeZone(QTimeZone(60*60*dateTimeData.mid(26, 3).toInt()));
    //Give back the target time.
    return targetTime;
}
