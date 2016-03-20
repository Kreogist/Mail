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

#include <QSplitter>

class KMMainWindowLeftContainer;
/*!
 * \brief The KMMainWindowContainer class provides a widget which could contains
 * all the widgets of the main window. It could manage the animations of all the
 * element widgets.\n
 * The container itself won't provide any content widgets, you have to set the
 * mail list widget and content viewer widget to the container.
 */
class KMMainWindowContainer : public QSplitter
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
    /*!
     * \brief Set the title bar widget.
     * \param titleBar The title bar widget pointer.
     */
    void setTitleBar(QWidget *titleBar);

    /*!
     * \brief Set the mail list widget.
     * \param mailList The mail list widget pointer.
     */
    void setMailList(QWidget *mailList);

    /*!
     * \brief Set the unibar widget.
     * \param uniBar The unibar widget pointer.
     */
    void setUniBar(QWidget *uniBar);

    /*!
     * \brief Set the mail component widget.
     * \param mailComponent Mail component widget pointer.
     */
    void setMailComponent(QWidget *mailComponent);

private:
    KMMainWindowLeftContainer *m_leftContainer;
    QWidget *m_mailComponent;
};

#endif // KMMAINWINDOWCONTAINER_H
