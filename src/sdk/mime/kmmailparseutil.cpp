#include <QTextCodec>

#include "kmquotedprintable.h"

#include "kmmailparseutil.h"

#include <QDebug>

QString KMMailParseUtil::parseEncoding(const QString &data)
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
