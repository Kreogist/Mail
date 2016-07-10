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
#ifndef KNACCOUNTWIDGETBUTTON_H
#define KNACCOUNTWIDGETBUTTON_H

#include <QAbstractButton>

/*!
 * \brief The KNAccountWidgetButton class provides the button for the account
 * widget. It will not only provides the animation of the opacity changes, but
 * also provides the rotate image.
 */
class KNAccountWidgetButton : public QAbstractButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountWidgetButton widget.
     * \param parent The parent widget.
     */
    explicit KNAccountWidgetButton(QWidget *parent = 0);

signals:

public slots:
};

#endif // KNACCOUNTWIDGETBUTTON_H
