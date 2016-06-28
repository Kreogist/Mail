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

void KMMailComponentWebEngine::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KMMailComponentContentBase::resizeEvent(event);
    //Update the browser size.
    m_browser->resize(size());
}
