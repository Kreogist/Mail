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

#include "kmmimemultipart.h"
#include "kmmimebase.h"

#include "kmmimeparser.h"

KMMimeBase *KMMimeParser::parseMimePart(const QStringList &content)
{
    //Initial the mime part.
    KMMimeBase *mimePart=nullptr;
    return mimePart;
}

KMMimeMultiPart *KMMimeParser::parseMultiMimePart(const QStringList &content)
{
//    //Find the last content part.
//    int lastSplitter=-1;
//    //Check all the content.
//    QString splitter="--"+contentBoundary();
//    //Initial a multi mime part.
//    KMMimeMultiPart *multiPart=nullptr;
//    //For all the content.
//    for(int i=0; i<content.size(); ++i)
//    {
//        //Check out splitter.
//        if(content.at(i)==splitter)
//        {
//            //Check last splitter.
//            if(lastSplitter==-1)
//            {
//                //Save the first splitter.
//                lastSplitter=i;
//                //Go to next splitter.
//                continue;
//            }
//            //Or else, we need to build the mime part.
//            KMMimeBase *mimePart=parseMimePart(content.mid(lastSplitter,
//                                                           i-lastSplitter));
//            //Check out mime part.
//            if(mimePart!=nullptr)
//            {
//                //Append to multi part.
//                if(multiPart==nullptr)
//                {
//                    //Generate a multi part.
//                    multiPart=new KMMimeMultiPart(nullptr);
//                }
//                //Append mime part to multi part.
//                multiPart->addPart(mimePart);
//            }
//            //Update the last splitter.
//            lastSplitter=i;
//        }
//    }
//    //Give back the mime part.
//    return multiPart;
    return nullptr;
}
