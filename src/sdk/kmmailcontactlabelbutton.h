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
#ifndef KMMAILCONTACTLABELBUTTON_H
#define KMMAILCONTACTLABELBUTTON_H

#include <QAbstractButton>

/*!
 * \brief The KMMailContactLabelButton class provides a button which will be
 * showed the contact information of one person's E-mail information and will
 * emitted signal for asking to show the card of this person.
 */
class KMMailContactLabelButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KMMailContactLabelButton widget.
     * \param parent The parent widget.
     */
    explicit KMMailContactLabelButton(QWidget *parent = 0);

    /*!
     * \brief Get the current contact avatar icon.
     * \return The avatar icon in pixmap format.
     */
    QPixmap avatarIcon() const;

    /*!
     * \brief Reimplemented from QAbstractButton::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QAbstractButton::setText().
     */
    void setText(const QString &text);

signals:

public slots:
    /*!
     * \brief Set the avatar icon.
     * \param avatarIcon The icon of the avatar.
     */
    void setAvatarIcon(const QPixmap &avatarIcon);

protected:
    /*!
     * \brief Reimplemented from QAbstractButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap m_avatarIcon;
};

#endif // KMMAILCONTACTLABELBUTTON_H
