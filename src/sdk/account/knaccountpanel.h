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
#ifndef KNACCOUNTPANEL_H
#define KNACCOUNTPANEL_H

#include <QWidget>

/*!
 * \brief The KNAccountPanel class provides a panel port for all the account
 * panels. It will provides all the necessary signals and functions which might
 * be used in a account panel widget. The account widget will managed use this
 * port class.
 */
class KNAccountPanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountPanel widget with given parent widget.
     * \param parent The parent widget pointer.
     */
    explicit KNAccountPanel(QWidget *parent = 0): QWidget(parent){}

signals:

public slots:
};

#endif // KNACCOUNTPANEL_H
