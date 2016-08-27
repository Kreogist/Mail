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
#ifndef KNGLOBAL_H
#define KNGLOBAL_H

#include <QObject>

/*!
 * \def knGlobal
 * A global pointer referring to the unique global object.
 */
#define knGlobal (KNGlobal::instance())

class KNConfigure;
class KNMainWindow;
/*!
 * \brief The KNGlobal class is a global instance which contains several public
 * function. \n
 * You should only construct this class once. If you construct the KNGlobal more
 * than once, the other instance will become no use.\n
 * This global instance will also initial several singleton instance, and take
 * those managers' control.
 *  * KNConfigureManager - A Json based operating system independent configure
 * system.
 *  * KNThemeManager - A Json based color theme file manager.
 *  * KNLocaleManager - International settings. Including translations and
 * locale default codec.
 *  * KNFontManager - A custom font files loader.
 */
class KNGlobal : public QObject
{
    Q_OBJECT
public:
    enum DefaultPath
    {
        KreogistDir,
        UserDataDir,
        ResourceDir,
        ContactDir,
        GeneralDir,
        AccountDir,
        DefaultDirCount
    };

    /*!
     * \brief Get the singleton instance of class KNGlobal.
     * \return The global instance of KNGlobal.
     */
    static KNGlobal *instance();

    /*!
     * \brief Initial the global class.
     * \param parent The parent object of the global class.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the specific directory path. The definition of each
     * directories.\n
     *   Kreogist Dir - This dir stores all the configure or database file of
     * Kreogist applications. This path cannot be changed.\n
     *   User Data Dir - This dir stores the configure file. This path cannot
     * be changed.\n
     *   Resource Dir - This dir stores the default system resource. Like some
     * official plugins. This path cannot be changed.\n
     *   Contact Dir - This dir stores the contact data.\n
     *   General Dir - This dir stores Kreogist Account information or some
     * public resource shared by Kreogist Application.
     * You should get the directory according to the DefaultPath enumerate.
     * \param index The directory enumerate number.
     * \return The path of the directory.
     */
    QString dirPath(const int &index) const;

    /*!
     * \brief Get the cache configure class.
     * \return The cache KNConfigure class.
     */
    KNConfigure *cacheConfigure();

    /*!
     * \brief Get the system configure class.
     * \return The system KNConfigure class.
     */
    KNConfigure *systemConfigure();

    /*!
     * \brief Get the user configure class.
     * \return The user KNConfigure class.
     */
    KNConfigure *userConfigure();

    /*!
     * \brief Get the account configure class.
     * \return The account KNConfigure class.
     */
    KNConfigure *accountConfigure();

    /*!
     * \brief Get the main window pointer.
     * \return The main window widget pointer. Default value is a null pointer.
     */
    KNMainWindow *mainWindow();

    /*!
     * \brief Set the new main window widget pointer.
     * \param mainWindow The main window pointer.
     */
    void setMainWindow(KNMainWindow *mainWindow);

signals:

public slots:
    /*!
     * \brief Update the infrastructure settings, after calling this slot, all
     * the settings will be updated.
     */
    void updateInfrastructure();

private:
    explicit KNGlobal(QObject *parent = 0);
    //Disable the copy of the instance.
    KNGlobal(const KNGlobal &);
    KNGlobal(KNGlobal &&);

    inline void initialInfrastrcture();
    inline void initialDefaultDirPath();

    //Global instance pointer.
    static KNGlobal *m_instance;

    QString m_dirPath[DefaultDirCount];
    KNConfigure *m_globalConfigure;
    KNMainWindow *m_mainWindow;
};

#endif // KNGLOBAL_H
