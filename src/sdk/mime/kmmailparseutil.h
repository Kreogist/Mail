#ifndef KMMAILPARSEUTIL_H
#define KMMAILPARSEUTIL_H

#include <QString>

class KMMailParseUtil
{
public:
    static QString parseEncoding(const QString &data);

private:
    KMMailParseUtil();
};

#endif // KMMAILPARSEUTIL_H
