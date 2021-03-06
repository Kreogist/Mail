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
#ifndef KNMAILWEBKITVIEWER_H
#define KNMAILWEBKITVIEWER_H

#include "knmailwebviewerbase.h"

class QWebView;
/*!
 * \brief The KNMailWebEngineViewer class provides the web viewer using Qt
 * WebEngine module.
 */
class KNMailWebkitViewer : public KNMailWebViewerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailWebEngineViewer widget.
     * \param parent The parent widget.
     */
    explicit KNMailWebkitViewer(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMailWebViewerBase::setTextContent().
     */
    void setTextContent(const QByteArray &content,
                        const QString &textType) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailWebViewerBase::setLocalUrl().
     */
    void setLocalUrl(const QString &url) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailWebViewerBase::reset().
     */
    void reset() Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMailWebViewerBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QWebView *m_browser;
};

#endif // KNMAILWEBKITVIEWER_H
