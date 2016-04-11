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
#ifndef KMTITLEBARCOMBO_H
#define KMTITLEBARCOMBO_H

#include <QAbstractButton>

class QTimeLine;
/*!
 * \brief The KNTitleBarCombo class provides the widget which could diplay the
 * avatar thumbnails and the folder icon and text.
 */
class KMTitleBarCombo : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNTitleBarCombo widget.
     * \param parent The parent widget.
     */
    explicit KMTitleBarCombo(QWidget *parent = 0);

    /*!
     * \brief Get the text that current title bar display.
     * \return The title bar text.
     */
    QString text() const;

    /*!
     * \brief Get the user avatar pixmap. If you never set the avatar, it will
     * return a null pixmap.
     * \return The user avatar pixmap.
     */
    QPixmap userAvatar() const;

signals:

public slots:
    /*!
     * \brief Set the user avatar that current Kreogist Account login.
     * \param userAvatar The account avatar.
     */
    void setUserAvatar(const QPixmap &userAvatar);

    /*!
     * \brief Set the text of the title bar will be display.
     * \param text The text.
     */
    void setText(const QString &text);

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionThemeChange();
    void onActionMouseInOut(int frame);

private:
    inline void startAnime(int targetBrightness);
    QLinearGradient m_highlight, m_shadowGradient;
    QPixmap m_userAvatar, m_anonymous, m_indicator;
    QColor m_dropShadow;
    QString m_text;
    QTimeLine *m_mouseInOut;
    int m_brightness;
    bool m_pressed;
};

#endif // KMTITLEBARCOMBO_H
