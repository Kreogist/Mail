#ifndef KMWEBKITCONTENTPARSER_H
#define KMWEBKITCONTENTPARSER_H

#include "kmmailcontentparser.h"

class KMWebkitContentParser : public KMMailContentParser
{
    Q_OBJECT
public:
    explicit KMWebkitContentParser(QObject *parent = 0);

    QString rawTextContent(const QString &filePath) Q_DECL_OVERRIDE;

signals:

public slots:

private:
};

#endif // KMWEBKITCONTENTPARSER_H
