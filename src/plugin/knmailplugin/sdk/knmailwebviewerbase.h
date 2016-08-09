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

#ifndef KNMAILWEBVIEWERBASE_H
#define KNMAILWEBVIEWERBASE_H

#include <QWidget>

/*!
 * \brief The KNMailWebViewerBase class provides a widget which is used to show
 * the web page using different kernel.
 */
class KNMailWebViewerBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief KNMailWebViewerBase
     * \param parent
     */
    KNMailWebViewerBase(QWidget *parent = 0) : QWidget(parent){}

signals:

public slots:
};

#endif // KNMAILWEBVIEWERBASE_H
