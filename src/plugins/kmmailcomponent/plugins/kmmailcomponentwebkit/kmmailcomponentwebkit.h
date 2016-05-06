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

#ifndef KMMAILCOMPONENTWEBKIT_H
#define KMMAILCOMPONENTWEBKIT_H

#include "../../sdk/kmmailcomponentcontentbase.h"

class QWebView;
/*!
 * \brief The KMMailComponentWebEngine class provides the mail browser using the
 * Qt WebEngine module.
 */
class KMMailComponentWebKit : public KMMailComponentContentBase
{
    Q_OBJECT
public:
    /*!
     * \brief KMMailComponentWebEngine
     * \param parent
     */
    explicit KMMailComponentWebKit(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KMMailComponentContentBase::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KMMailComponentContentBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QWebView *m_browser;
};

#endif // KMMAILCOMPONENTWEBKIT_H
