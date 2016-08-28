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
#ifndef KNPLUGINMANAGER_H
#define KNPLUGINMANAGER_H

#include <QStringList>

#include <QObject>

class KNMainWindow;
class KNMainWindowLeftBarBase;
class KNMailPluginBase;
/*!
 * \brief The KNPluginManager class is a port-class manager. It will create and
 * load all the plugin classes with the specific port class. It will also be
 * responsible for linking all the signals between these classes.
 */
class KNPluginManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct the KNPluginManager with parent object.
     * \param parent The owner of the plugin manager. Normally, this should be
     * empty. You should construct the plugin manager in the main() function.
     */
    explicit KNPluginManager(QObject *parent = 0);
    ~KNPluginManager();

signals:
    /*!
     * \brief When the application is launching, this signal will be emitted.
     */
    void requireLaunch();

public slots:
    /*!
     * \brief Set the application main window class.\n
     * Only the first main window object will be set. When you called this
     * function more than once, it will ignore those request except the first
     * time.
     * \param mainWindow The main window class.
     */
    void setMainWindow(KNMainWindow *mainWindow);

    /*!
     * \brief Load all the plugins.
     */
    void loadPlugins();

    /*!
     * \brief Start to launch the application. It will check the state and
     * decide to show main window or showing other stuffs.
     */
    void launchApplication();

    /*!
     * \brief When there's a new arguments valid, this slot will be called. It
     * will send the arguments to the category plugin.
     * \param arguments The arguments from system or from other instance.
     */
    void onActionArgumentsAvaliable(QStringList arguments);

private slots:

private:
    inline void setApplicationInformation();
    void loadLeftBar(KNMainWindowLeftBarBase *leftBar);
    void loadMailPlugin(KNMailPluginBase *mailPlugin);
    KNMainWindowLeftBarBase *m_leftBar;
};

#endif // KNPLUGINMANAGER_H
