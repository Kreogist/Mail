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

#include "knsideshadowwidget.h"

#include "kmmailcomponentcontent.h"

#include <QDebug>

#define ShadowHeight 18

KMMailComponentContent::KMMailComponentContent(QWidget *parent) :
    QWidget(parent),
    m_browser(new QWebEngineView(this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadowWidget::TopShadow,
                                       this)),
    m_documentHeight(0)
{
    //Set properties.
    setMinimumHeight(ShadowHeight);
    //Set the fixed top shadow height.
    m_topShadow->setFixedHeight(ShadowHeight);

    //Configure the settings.
    QWebEngineSettings *settings=m_browser->settings();
    settings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled,
                           true);
    settings->setAttribute(QWebEngineSettings::PluginsEnabled,
                           true);
    settings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,
                           true);
    //Configure the browser.
    m_browser->load(QUrl("http://www.google.com.au"));
}

QSize KMMailComponentContent::sizeHint() const
{
    return m_browser->sizeHint();
}

void KMMailComponentContent::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Re-pos the top shadow.
    m_topShadow->resize(width(), ShadowHeight);
    //Update the browser size.
    m_browser->resize(size());
}
