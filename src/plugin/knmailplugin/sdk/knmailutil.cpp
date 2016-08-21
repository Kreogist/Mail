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

#include "knmailutil.h"

#include <QDebug>

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
