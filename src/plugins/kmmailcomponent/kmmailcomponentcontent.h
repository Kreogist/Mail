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
#ifndef KMMAILCOMPONENTCONTENT_H
#define KMMAILCOMPONENTCONTENT_H

#include <QWidget>

class QWebEngineView;
class KNSideShadowWidget;
/*!
 * \brief The KMMailComponentContent class provides a widget to show the content
 * of the .
 */
class KMMailComponentContent : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailComponentContent widget.
     * \param parent The parent widget.
     */
    explicit KMMailComponentContent(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QWidget::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QWebEngineView *m_browser;
    KNSideShadowWidget *m_topShadow;
    int m_documentHeight;
};

#endif // KMMAILCOMPONENTCONTENT_H
