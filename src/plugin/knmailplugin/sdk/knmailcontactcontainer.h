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

#ifndef KNMAILCONTACTCONTAINER_H
#define KNMAILCONTACTCONTAINER_H

#include <QList>

#include <QWidget>

#define LayoutSpacing 5

class KNMailContactButton;
class KNMailContactFlowLayout;
/*!
 * \brief The KNMailContactContainer class is a widget which will be used to
 * hold the contact button, it contains the contact button .
 */
class KNMailContactContainer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailContactContainer widget.
     * \param parent The parent widget.
     */
    explicit KNMailContactContainer(QWidget *parent = 0);

    /*!
     * \brief Append contact button to the widget.
     * \param button The contact button.
     */
    void addContact(KNMailContactButton *button);

    /*!
     * \brief Add the E-mail address to the contact list.
     * \param email The contact address.
     * \param caption The contact caption. The default value is a null string.
     */
    void addContact(const QString &email, const QString &caption=QString());

    /*!
     * \brief Reimplemented from QWidget::hasHeightForWidth().
     */
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::heightForWidth().
     */
    int heightForWidth(int targetWidth) const Q_DECL_OVERRIDE;

    /*!
     * \brief Update all the contact button palette.
     * \param pal The contact button palette.
     */
    void setContactPalette(const QPalette &pal);

signals:

public slots:
    /*!
     * \brief Remove all the buttons in the container.
     */
    void clear();

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QList<KNMailContactButton *> m_buttons;
    QPalette m_buttonPalette;
    KNMailContactFlowLayout *m_mainLayout;
};

#endif // KNMAILCONTACTCONTAINER_H
