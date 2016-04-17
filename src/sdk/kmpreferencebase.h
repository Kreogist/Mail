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
#ifndef KMPREFERENCEBASE_H
#define KMPREFERENCEBASE_H

#include <QWidget>

/*!
 * \brief The KMPreferenceBase class provides the ports widget of the preference
 * widget. All the preference widget should achieve all the ports in this port.
 * \n
 * The preference widget is used to display all the panel in this widget.
 */
class KMPreferenceBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMPreferenceBase widget.
     * \param parent The parent widget.
     */
    KMPreferenceBase(QWidget *parent = 0): QWidget(parent){}

signals:

public slots:

};

#endif // KMPREFERENCEBASE_H
