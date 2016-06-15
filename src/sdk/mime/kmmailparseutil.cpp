#include <QTextCodec>

#include "kmquotedprintable.h"

#include "kmmailparseutil.h"

#include <QDebug>

#define EncodingStartMark "=?"
#define EncodingEndMark "?="

QString KMMailParseUtil::parseEncoding(QString data)
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
                //It is not followed by a space.
                (data.at(dataEnd+2)!=' '))
        {
            //Find the next data end.
            dataEnd=data.indexOf(EncodingEndMark,
                                 dataEnd+1);
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
            //Check data is start with a spacing or not, if so, remove the spacing.
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

QString KMMailParseUtil::parseEncodingPart(const QString &data)
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
                        KMQuotedPrintable::decode(
                            content.mid(secondQuestionMark+1)));
        }
        return content;
    }
    //Or else, we don't need to parse.
    return data;
}
