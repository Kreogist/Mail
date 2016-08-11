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
#ifndef KNMAILCONTACTLIST_H
#define KNMAILCONTACTLIST_H

#include <QScrollArea>

class KNMailContactButton;
class KNRoundedBorderButton;
class KNMailContactContainer;
/*!
 * \brief The KNMailContactList class provides the list of contact button. This
 * is a widget which holds the contact list widget. It will show the scroll bar
 * for expanded state.
 */
class KNMailContactList : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailContactList widget.
     * \param parent The parent widget.
     */
    explicit KNMailContactList(QWidget *parent = 0);

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
     * \brief Update all the contact button palette.
     * \param pal The contact button palette.
     */
    void setContactPalette(const QPalette &pal);

signals:

public slots:
    /*!
     * \brief Set the expand state of the contact list.
     * \param isExpand To expand the contact list, set this value to true. If
     * the list is not expandable, then this function won't work.
     */
    void setExpandState(bool isExpand);

    /*!
     * \brief Remove all the buttons in the container.
     */
    void clear();

protected:
    /*!
     * \brief Reimplemented from QScrollArea::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QScrollArea::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    inline void updateExpandButton();
    KNMailContactContainer *m_container;
    KNRoundedBorderButton *m_moreButton;
    bool m_isExpand;
};

#endif // KNMAILCONTACTLIST_H
