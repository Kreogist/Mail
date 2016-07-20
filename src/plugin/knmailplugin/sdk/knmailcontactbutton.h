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
#ifndef KNMAILCONTACTBUTTON_H
#define KNMAILCONTACTBUTTON_H

#include <QWidget>

/*!
 * \brief The KNMailContactButton class provides the contact button widget for
 * the mail viewer title. It will provides a button for display the contact
 * operation menu.
 */
class KNMailContactButton : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailContactButton widget.
     * \param parent The parent widget.
     */
    explicit KNMailContactButton(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // KNMAILCONTACTBUTTON_H
