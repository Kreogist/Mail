#ifndef KMMAILCONTENTPARSER_H
#define KMMAILCONTENTPARSER_H

#include <QObject>

class KMMailContentParser : public QObject
{
    Q_OBJECT
public:
    KMMailContentParser(QObject *parent = 0) : QObject(parent){}

    virtual QString rawTextContent(const QString &filePath)=0;

signals:

public slots:
};

#endif // KMMAILCONTENTPARSER_H
