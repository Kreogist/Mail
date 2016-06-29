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

#ifndef KMMAILCOMPONENTWEBENGINE_H
#define KMMAILCOMPONENTWEBENGINE_H

#include "../../sdk/kmmailcomponentcontentbase.h"

class QWebEngineView;
class KMMimePart;
/*!
 * \brief The KMMailComponentWebEngine class provides the mail browser using the
 * Qt WebEngine module.\n
 * This module will use QWebEngineView as the displayer of the web site. It will
 * be enabled when Qt version is greater than 5.6 for 64-bit platforms. 32-bit
 * platforms please use QWebView which is already out-of-date.
 */
class KMMailComponentWebEngine : public KMMailComponentContentBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailComponentWebEngine object with given parent
     * object.
     * \param parent The parent object.
     */
    explicit KMMailComponentWebEngine(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KMMailComponentContentBase::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief reset
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief setMimePart
     * \param mimePart
     */
    void setMimePart(KMMimePart *mimePart) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KMMailComponentContentBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    inline void loadPlainText(KMMimePart *mimePart);
    inline void loadHtml(KMMimePart *mimePart,
                         const QString &prefix=QString());
    QWebEngineView *m_browser;
    KMMimePart *m_mimePart;
};

#endif // KMMAILCOMPONENTWEBENGINE_H
