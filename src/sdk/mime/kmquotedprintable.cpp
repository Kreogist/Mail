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

QString KMQuotedPrintable::encode(const QByteArray &data)
{
    //Initial the hex data.
    static const char hex[]={'0', '1', '2', '3', '4', '5', '6', '7',
                             '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    //Prepare the encoded text.
    QString encodedText;
    char byte;
    //Translate all raw data.
    for(int i=0; i<data.size(); ++i)
    {
        //Get the current byte.
        byte=data.at(i);
        //Check the byte data.
        if ((byte == 0x20) || ((byte >= 33) && (byte <= 126) && (byte != 61)))
        {
            //Simply append the encoded text.
            encodedText.append(byte);
        }
        else
        {
            //Translate the byte to encoded code.
            encodedText.append('=');
            encodedText.append(hex[((byte >> 4) & 0x0F)]);
            encodedText.append(hex[(byte & 0x0F)]);
        }
    }
    //Give back the encoded text.
    return encodedText;
}

QByteArray KMQuotedPrintable::decode(const QString &text)
{
    //Initial the hex value.
    //                           0  1  2  3  4  5  6  7  8  9  :  ;  <
    static const int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0,
                              // =  >  ?  @  A   B   C   D   E   F
                                 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
    //Prepare the raw data cache.
    QByteArray rawData;
    //Check all data.
    for (int i = 0; i<text.size(); ++i)
    {
        //When the text is =, means we need to translate data.
        if(text.at(i)=='=')
        {
            //Append the target char to raw data.
            rawData.append((hexVal[text.at(i + 1).toLatin1() - '0'] << 4) +
                            hexVal[text.at(i + 2).toLatin1() - '0']);
            //Move i.
            i+=2;
            //Go to next char.
            continue;
        }
        //Simply append the data.
        rawData.append(text.at(i).toLatin1());
    }
    //Give back the raw data.
    return rawData;
}
