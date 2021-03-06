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
 * along with this program; if not, write to the Free Software
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMAILWEBVIEWERBASE_H
#define KNMAILWEBVIEWERBASE_H

#include <QWidget>

/*!
 * \brief The KNMailWebViewerBase class provides a widget which is used to show
 * the web page using different kernel.
 * This port is built for different core of Qt. Different environment and
 * different compiler will use different module to realize the web page
 * rendering data.
 */
class KNMailWebViewerBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailWebViewerBase widget.
     * \param parent The parent widget.
     */
    KNMailWebViewerBase(QWidget *parent = 0) : QWidget(parent){}

signals:

public slots:
    /*!
     * \brief Display plain content in the viewer.
     * \param content The display text.
     * \param textType The text format.
     */
    virtual void setTextContent(const QByteArray &content,
                                const QString &textType)=0;

    /*!
     * \brief Set the parsed file url at local.
     * \param url The url at local directory.
     */
    virtual void setLocalUrl(const QString &url)=0;

    /*!
     * \brief Clear all the content of the viewer.
     */
    virtual void reset()=0;
};

#endif // KNMAILWEBVIEWERBASE_H
