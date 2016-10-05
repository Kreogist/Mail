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
#include <QtWebEngineWidgets/QWebEngineView>

#include "knmailwebengineviewer.h"

KNMailWebEngineViewer::KNMailWebEngineViewer(QWidget *parent) :
    KNMailWebViewerBase(parent),
    m_browser(new QWebEngineView(this))
{
}

void KNMailWebEngineViewer::setTextContent(const QByteArray &content,
                                           const QString &textType)
{
    //Check text type.
    if(textType.contains("plain"))
    {
        //Set the plain text content, suppose all the content is encoding in
        //UTF-8.
        m_browser->setHtml("<HTML> <HEAD></HEAD> <BODY> <PLAINTEXT>" +
                           content);
        //Complete.
        return;
    }
    //Use set content to process other type text.
    m_browser->setContent(content, textType);
}

void KNMailWebEngineViewer::setLocalUrl(const QString &url)
{
    //Set the url to browser.
    m_browser->setUrl(QUrl::fromLocalFile(url));
}

void KNMailWebEngineViewer::reset()
{
    //Clear the browser content.
    m_browser->load(QUrl("about:blank"));
}

void KNMailWebEngineViewer::resizeEvent(QResizeEvent *event)
{
    //Update the widget size.
    KNMailWebViewerBase::resizeEvent(event);
    //Update the browser size.
    m_browser->resize(size());
}
