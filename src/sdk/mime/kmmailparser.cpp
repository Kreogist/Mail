#include <QFile>
#include <QTextStream>

#include "kmmailparseutil.h"

#include "kmmailparser.h"

#include <QDebug>

KMMailParser::KMMailParser(QObject *parent) : QObject(parent)
{

}

void KMMailParser::parseFile(const QString &filePath,
                             QString &receiver,
                             QString &subject)
{
    //Get the file.
    QFile emlFile(filePath);
    //Check exist and open.
    if((!emlFile.exists()) ||
            (!emlFile.open(QIODevice::ReadOnly)))
    {
        //Cannot find or open the file.
        return;
    }
    //Read from the first line.
    QTextStream emlFileReader(&emlFile);
    //Prepare the read line cache.
    QString lineCache;
    //Prepare the content start flag.
    bool subjectStart=false;
    //Read until the last line.
    while(emlFileReader.readLineInto(&lineCache, 0))
    {
        //Check the subject start flag.
        if(subjectStart && lineCache.startsWith(" "))
        {
            //Append subject data.
            subject.append(KMMailParseUtil::parseEncoding(lineCache.mid(1)));
            //Goto next line.
            continue;
        }
        //Once go here, means subject is end.
        subjectStart=false;
        //Still contains data.
        if(lineCache.isEmpty())
        {
            //Content start, mission complete.
            return;
        }
        //Parse the data.
        if(lineCache.startsWith("From: "))
        {
            //Save the receiver data.
            QString receiverData=lineCache.mid(6);
            //Find the address start.
            int addrressStart=receiverData.indexOf(" <");
            //Get the receiver.
            receiver=KMMailParseUtil::parseEncoding(
                        receiverData.left(addrressStart)) +
                    receiverData.mid(addrressStart);
            //Go to next line.
            continue;
        }
        if(lineCache.startsWith("Subject: "))
        {
            //Get the subject.
            subject=KMMailParseUtil::parseEncoding(lineCache.mid(9));
            //Start the subject.
            subjectStart=true;
            //Go to next line.
            continue;
        }
    }
    //Close the file.
    emlFile.close();
}

