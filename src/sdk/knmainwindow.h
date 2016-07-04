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
#ifndef KNMAINWINDOW_H
#define KNMAINWINDOW_H

#include <QMainWindow>

/*!
 * \brief The KNMainWindow class provides the main window of the Mail
 * application. It will host for all the visible widget. It might be hide but it
 * will be constructed everytime when launch the application.
 */
class KNMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMainWindow widget.
     * \param parent The parent widget.
     */
    explicit KNMainWindow(QWidget *parent = 0);

signals:

public slots:
};

#endif // KNMAINWINDOW_H
