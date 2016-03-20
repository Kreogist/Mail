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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KMMAINWINDOWCONTAINER_H
#define KMMAINWINDOWCONTAINER_H

#include <QWidget>

/*!
 * \brief The KMMainWindowContainer class provides a widget which could contains
 * all the widgets of the main window. It could manage the animations of all the
 * element widgets.\n
 * The container itself won't provide any content widgets, you have to set the
 * mail list widget and content viewer widget to the container.
 */
class KMMainWindowContainer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMainWindowContainer widget with given parent.
     * \param parent The parent widget pointer.
     */
    explicit KMMainWindowContainer(QWidget *parent = 0);

signals:

public slots:

private:
    QWidget *m_titleBar, *m_mailList, *m_uniBar, *m_mailComponent;
};

#endif // KMMAINWINDOWCONTAINER_H
