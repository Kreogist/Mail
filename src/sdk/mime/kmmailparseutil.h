#ifndef KMMAILPARSEUTIL_H
#define KMMAILPARSEUTIL_H

#include <QString>

class KMMailParseUtil
{
public:
    static QString parseEncoding(QString data);

private:
    static QString parseEncodingPart(const QString &data);
    KMMailParseUtil();
};

#endif // KMMAILPARSEUTIL_H
