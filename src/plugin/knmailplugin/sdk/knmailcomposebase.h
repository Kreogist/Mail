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
#ifndef KNMAILCOMPOSEBASE_H
#define KNMAILCOMPOSEBASE_H

#include <QWidget>

/*!
 * \brief The KNMailComposeBase class provides the basic information about the
 * compose widget window. This widget will be used for composing the
 */
class KNMailComposeBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailComposeBase widget.
     * \param parent The parent widget.
     */
    KNMailComposeBase(QWidget *parent = 0) : QWidget(parent){}

signals:

public slots:
};

#endif // KNMAILCOMPOSEBASE_H
