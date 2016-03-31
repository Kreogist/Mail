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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KMTITLEBARBUTTON_H
#define KMTITLEBARBUTTON_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KMTitleBarButton class is a button which could be used on the
 * title bar. This button shouldn't be shown too much.\n
 * It should be fixed size.
 */
class KMTitleBarButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMTitleBarButton button widget.
     * \param parent The parent widget.
     */
    explicit KMTitleBarButton(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionThemeChange();
    void onActionMouseInOut(int frame);

private:
    inline void startAnime(int targetBrightness);
    QLinearGradient m_highlight;
    QColor m_dropShadow;
    QTimeLine *m_mouseInOut;
    int m_brightness;
};

#endif // KMTITLEBARBUTTON_H
