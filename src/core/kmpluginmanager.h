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

#ifndef KMPLUGINMANAGER_H
#define KMPLUGINMANAGER_H

#include <QObject>

class KMMainWindow;
/*!
 * \brief The KMPluginManager class is the global object which could manage all
 * the plugins. And it will configure the plugins of UI and functions.
 */
class KMPluginManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KMPluginManager object.
     * \param parent The parent object.
     */
    explicit KMPluginManager(QObject *parent = 0);
    ~KMPluginManager();

    /*!
     * \brief Get the application main window.
     * \return The main window class.
     */
    KMMainWindow *mainWindow() const;

signals:

public slots:
    /*!
     * \brief Set the application main window class.\n
     * Only the first main window object will be set. When you called this
     * function more than once, it will ignore those request except the first
     * time.
     * \param mainWindow The main window class.
     */
    void setMainWindow(KMMainWindow *mainWindow);

    /*!
      * \brief Load all the plugins.
      */
     void loadPlugins();

     /*!
      * \brief Show the main window, start to launch the application.
      */
     void launchApplication();

     /*!
      * \brief When there's a new arguments valid, this slot will be called. It
      * will send the arguments to the category plugin.
      * \param arguments The arguments from system or from other instance.
      */
     void onActionArgumentsAvaliable(QStringList arguments);

private:
    inline void setApplicationInformation();
    KMMainWindow *m_mainWindow;
};

#endif // KMPLUGINMANAGER_H
