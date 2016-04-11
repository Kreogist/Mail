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
#ifndef KMMAILCONTACTBUTTON_H
#define KMMAILCONTACTBUTTON_H

#include <QAbstractButton>

class KMMailContactLabelButton;
class KMMailContactAddButton;
/*!
 * \brief The KMMailContactButton class provides the button which could provide
 * a button which could be used.
 */
class KMMailContactButton : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailContactButton widget.
     * \param parent The parent widget.
     */
    explicit KMMailContactButton(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from QWidget::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /*!
     * \brief Get the current contact avatar icon.
     * \return The avatar icon in pixmap format.
     */
    QPixmap avatarIcon() const;

    /*!
     * \brief Whether this contact information is already exist in the contact
     * list.
     * \return Whether the exist in the contact list.
     */
    bool exist() const;

signals:

public slots:
    /*!
     * \brief Set whether this button should show the add button.
     * \param exist To show the add button, set it to false.
     */
    void setExist(bool exist);

    /*!
     * \brief Set the avatar icon.
     * \param avatarIcon The icon of the avatar.
     */
    void setAvatarIcon(const QPixmap &avatarIcon);

    /*!
     * \brief Set the E-mail address.
     * \param mailAddress The mail address of the contact.
     */
    void setContactAddress(const QString &mailAddress);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    KMMailContactAddButton *m_addButton;
    KMMailContactLabelButton *m_labelButton;
    bool m_exist;
};

#endif // KMMAILCONTACTBUTTON_H
