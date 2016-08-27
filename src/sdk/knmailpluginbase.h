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
#ifndef KNMAILPLUGINBASE_H
#define KNMAILPLUGINBASE_H

#include <QWidget>

/*!
 * \brief The KNMailPluginBase class provides the basic port functions of a mail
 * plugin class should implemented this class and realize all the virtual
 * functions.
 */
class KNMailPluginBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailPluginBase widget with given parent.
     * \param parent The parent widget.
     */
    KNMailPluginBase(QWidget *parent = 0):QWidget(parent){}

    /*!
     * \brief Provides the left bar panel of the mail plugin, it should display
     * all the information of the managing account.
     * \return The account panel widget pointer.
     */
    virtual QWidget *accountPanel()=0;

    /*!
     * \brief Load the plugins of the mail plugin.
     */
    virtual void loadPlugins() = 0;

    /*!
     * \brief Get the compose button widget pointer.
     * \return The compose button widget pointer.
     */
    virtual QWidget *composeButton() const=0;

signals:

public slots:
    /*!
     * \brief Save the account data.
     */
    virtual void saveAccount()=0;
};

#endif // KNMAILPLUGINBASE_H
