#ifndef KMMAILPARSER_H
#define KMMAILPARSER_H

#include <QObject>

/*!
 * \brief The KMMailParser class will do the quick parse for a eml or emlx file.
 * It will do the quick read for the receiver, subject and get the brief
 * information.
 */
class KMMailParser : public QObject
{
    Q_OBJECT
public:
    static void parseFile(const QString &filePath,
                          QString &receiver,
                          QString &subject);
signals:

public slots:

private:
    /*!
     * \brief Construct a KMMailParser object.
     * \param parent The parent object.
     */
    explicit KMMailParser(QObject *parent = 0);
};

#endif // KMMAILPARSER_H
