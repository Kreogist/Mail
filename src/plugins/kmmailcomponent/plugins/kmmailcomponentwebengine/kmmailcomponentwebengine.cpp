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
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QTextCodec>

#include "mime/kmmailparseutil.h"
#include "mime/kmmimepart.h"
#include "mime/kmmimemultipart.h"

#include "kmmailcomponentwebengine.h"

KMMailComponentWebEngine::KMMailComponentWebEngine(QWidget *parent) :
    KMMailComponentContentBase(parent),
    m_browser(new QWebEngineView(this))
{
    //Configure the browser.
    m_browser->lower();
    //Configure the settings.
    QWebEngineSettings *settings=m_browser->settings();
    settings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled,
                           true);
    settings->setAttribute(QWebEngineSettings::PluginsEnabled,
                           true);
    settings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,
                           true);
}

QSize KMMailComponentWebEngine::sizeHint() const
{
    return m_browser->sizeHint();
}

void KMMailComponentWebEngine::reset()
{
    //Reset the broswer.
    m_browser->setUrl(QUrl("about:blank"));
}

void KMMailComponentWebEngine::setMimePart(KMMimePart *mimePart)
{
    //Reset the browser.
    reset();
    //Save the mime part.
    m_mimePart=mimePart;
    //Set content to the mime part.
    if(m_mimePart->isMultipart())
    {
        //Recast the mime part to multi mime part.
        KMMimeMultiPart *multiMimePart=
                static_cast<KMMimeMultiPart *>(m_mimePart);
        //Check all the mime part.
        for(int i=0, partCount=multiMimePart->partCount();
            i<partCount;
            ++i)
        {
            //Get the current item.
            KMMimePart *part=multiMimePart->part(i);
            //Check the content type.
            QString &&contentType=part->mimeProperty("content-type").toLower();
            //Check the content type.
            if(contentType.contains("text/html"))
            {
                //Load the html data.
                loadHtml(part);
            }
            else if(contentType.contains("text/plain"))
            {
                //Load the plain text.
                loadPlainText(part);
            }
        }
        //Use multipart parsing.
        return;
    }
    //Check the content type.
    QString &&contentType=m_mimePart->mimeProperty("content-type");
    if(contentType.contains("text/plain"))
    {
        //Load the plain text.
        loadPlainText(m_mimePart);
        //Mission complete.
        return;
    }
}

void KMMailComponentWebEngine::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KMMailComponentContentBase::resizeEvent(event);
    //Update the browser size.
    m_browser->resize(size());
}

inline void KMMailComponentWebEngine::loadPlainText(KMMimePart *mimePart)
{
    loadHtml(mimePart, "<HTML> <HEAD></HEAD> <BODY> <PLAINTEXT>");
}

inline void KMMailComponentWebEngine::loadHtml(KMMimePart *mimePart,
                                               const QString &prefix)
{
    //Get the charset.
    QMap<QString, QString> typeInfo;
    {
        //Split the content type data.
        QStringList contentData=
                mimePart->mimeProperty("content-type").split(';');
        //Check all the content data.
        foreach(auto i, contentData)
        {
            //Find the '='.
            int equalMark=i.indexOf('=');
            //Check the equal mark.
            if(equalMark==-1 || equalMark==i.size()-1)
            {
                //It doesn't contains the settings data.
                continue;
            }
            //Get the field name and value.
            typeInfo.insert(i.left(equalMark).simplified().toLower(),
                            i.mid(equalMark+1).simplified());
        }
    }
    //Decode the mime part data.
    QByteArray &&decodedContent=KMMailParseUtil::parseContent(
                mimePart->mimeProperty(
                    "content-transfer-encoding").toUpper(),
                mimePart->content());
    //Get the text codec.
    QTextCodec *targetCodec=QTextCodec::codecForName(
                typeInfo.value("charset").toUpper().toLatin1());
    //Check the target codec.
    if(targetCodec)
    {
        //Set the content directly to the browser.
        m_browser->setHtml(prefix +targetCodec->toUnicode(decodedContent));
        //Mission complete.
        return;
    }
    //Or else, directly set the content to the html.
    m_browser->setHtml(prefix+decodedContent);
}
