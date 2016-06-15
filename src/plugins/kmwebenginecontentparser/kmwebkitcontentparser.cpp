#include <QWebPage>
#include <QWebFrame>
#include <QScopedPointer>

#include "kmwebkitcontentparser.h"

KMWebkitContentParser::KMWebkitContentParser(QObject *parent) :
    KMMailContentParser(parent)
{

}

QString KMWebkitContentParser::rawTextContent(const QString &filePath)
{
    //Initial the web page.
    QScopedPointer<QWebPage> parser(new QWebPage());
    //Enabled MHTML support.
    ;
    //Initial the parser.
    parser->mainFrame()->load(QUrl::fromLocalFile(filePath));
    //Get the html data back.
    return parser->mainFrame()->toPlainText();
}

