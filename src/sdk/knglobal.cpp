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
#include <QApplication>
#include <QStandardPaths>

//Dependencies
#include "knutil.h"

//Managers
#include "knthememanager.h"
#include "knfontmanager.h"
#include "knlocalemanager.h"
#include "knconfigure.h"
#include "knconfiguremanager.h"

#include "knglobal.h"

KNGlobal *KNGlobal::instance()
{
    //Return the instance pointer.
    return m_instance;
}

void KNGlobal::initial(QObject *parent)
{
    //Check if the singleton instance variable is null. Set the pointer to this
    //object if this is the first constructed object.
    if(m_instance==nullptr)
    {
        m_instance=new KNGlobal(parent);
    }
}

KNConfigure *KNGlobal::cacheConfigure()
{
    return knConf->configure(KNConfigureManager::Cache);
}

KNConfigure *KNGlobal::systemConfigure()
{
    return knConf->configure(KNConfigureManager::System);
}

KNConfigure *KNGlobal::userConfigure()
{
    return knConf->configure(KNConfigureManager::User);
}

KNConfigure *KNGlobal::accountConfigure()
{
    return knConf->configure(KNConfigureManager::Account);
}

void KNGlobal::updateInfrastructure()
{
    ;
}

KNGlobal::KNGlobal(QObject *parent) :
    QObject(parent)
{
    //Initial the managers.
    //Gerenate the configure manager.
    KNConfigureManager::initial(this);
    //Generate the font manager.
    KNFontManager::initial(this);
    //Generate the locale manager.
    KNLocaleManager::initial(this);
    //Generate the theme manager.
    KNThemeManager::initial(this);
    //Generate the notification manager.
//    KNNotification::initial(this);
    //Generate the account system.
//    KNAccount::initial();

    //Initial the infrastructure.
    initialInfrastrcture();
}

inline void KNGlobal::initialInfrastrcture()
{
    //Initial the default path.
    initialDefaultDirPath();

    //Initial the configure manager.
    //Set the configure folder path.
    //-- Q: Why set configure path here?
    //   A: Because we won't change the configure path.
    //Because KNConfigureManager will automatically reload the configure, so we
    //don't need to load the configure here.
    knConf->setFolderPath(m_dirPath[UserDataDir]+"/Configure",
                          m_dirPath[AccountDir]);
    //Get the global configure.
    m_globalConfigure=userConfigure()->getConfigure("Global");

    //Initial the font manager.
    //Set the font resource folder.
    knFont->loadCustomFontFolder(m_dirPath[ResourceDir]+"/Fonts");
    //Set the default font.
    knFont->loadDefaultFont();

    //Initial the locale manager.
    //Load the language in language folder.
#ifdef Q_OS_LINUX
    //Thanks for Sou Bunnbu, amazingfate:
    //For Linux, we should also find langauges at /usr/share/Kreogist/mu.
    knI18n->addLanguageDirectory("/usr/share/Kreogist/mu/Language");
    //And the user installed languages, thanks for 1dot75cm.
    knI18n->addLanguageDirectory(m_dirPath[UserDataDir]+"/Language");
    knI18n->loadLanguageFiles();
#else
    //For Windows and Mac OS X, we can simply check the resource folder and
    //application dir.
    knI18n->addLanguageDirectory(m_dirPath[ResourceDir]+"/Language");
    knI18n->addLanguageDirectory(qApp->applicationDirPath()+"/Language");
    knI18n->loadLanguageFiles();
#endif
    //Load the current language file.
    //We will load the langauge file twice, for the first time, we have to load
    //the default language according to the system locale settings.
    knI18n->setDefaultLanguage();
    //Load the language stored in the configure file.
    knI18n->setLanguage(m_globalConfigure->data("Language").toString());

    //Initial the theme manager.
    //Load the theme in theme folder. It's familiar with the language folder.
#ifdef Q_OS_LINUX
    knTheme->loadThemeFiles("/usr/share/Kreogist/mu/Theme");
#else
    knTheme->loadThemeFiles(m_dirPath[ResourceDir]+"/Theme");
#endif
    //Load the current theme file.
    //Like the language, we will load theme twice. Default first, user next.
    knTheme->setTheme(0);
    //Load the theme in the configure file.
    knTheme->setTheme(m_globalConfigure->data("Theme").toString());

//    //Set the configure.
//    knAccount->setCacheConfigure(accountConfigure());
//    //Initial the account system.
//    knAccount->setWorkingThread(m_accountThread);
//    //Link the account.
//    connect(this, &KNGlobal::startWorking, knAccount, &KNAccount::startToWork,
//            Qt::QueuedConnection);
//    //Start account working thread.
//    m_accountThread->start();

    //Update infrastructure, update the path of the library directory.
    updateInfrastructure();
}

inline void KNGlobal::initialDefaultDirPath()
{
    /*
     * Initial the default path of the dirs.
     * Kreogist Dir:
     *     Windows: My documents/Kreogist/
     *    Mac OS X: Documents/Kreogist/
     *       Linux: ~/.kreogist/
     * Resource Dir:
     *     Windows: Application path
     *    Mac OS X: Application path/../Resources
     *       Linux: ~/.kreogist/mail
     * User Data Dir:
     *     $KreogistDir$/Mail
     * Contact Dir:
     *     $KreogistDir$/Contact
     * General Dir:
     *     $KreogistDir$/General
     * Kreogist Account Info:
     *      $GeneralDir$/account.info
     */
#ifdef Q_OS_WIN //No matter Win32/Win64
    m_dirPath[KreogistDir]=
            KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                       QStandardPaths::DocumentsLocation)
                                   +"/Kreogist");
    m_dirPath[ResourceDir]=qApp->applicationDirPath()+"/Resources";
#endif
#ifdef Q_OS_MACX
    m_dirPath[KreogistDir]=
            KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                       QStandardPaths::DocumentsLocation)
                                   +"/Kreogist");
    m_dirPath[ResourceDir]=
            KNUtil::simplifiedPath(qApp->applicationDirPath()+"/../Resources");
#endif
#ifdef Q_OS_LINUX
    m_dirPath[KreogistDir]=
            KNUtil::simplifiedPath(QStandardPaths::writableLocation(
                                       QStandardPaths::HomeLocation))
            + "/.kreogist";
    m_dirPath[ResourceDir]=m_dirPath[KreogistDir]+"/Resources";
#endif
    m_dirPath[AccountDir]=
            KNUtil::simplifiedPath(m_dirPath[KreogistDir]+"/Account");
    m_dirPath[UserDataDir]=
            KNUtil::simplifiedPath(m_dirPath[KreogistDir]+"/Mail");
    m_dirPath[ContactDir]=
            KNUtil::simplifiedPath(m_dirPath[KreogistDir]+"/Contact");
    m_dirPath[GeneralDir]=
            KNUtil::simplifiedPath(m_dirPath[KreogistDir]+"/General");
}

KNMainWindow *KNGlobal::mainWindow() const
{
    return m_mainWindow;
}

void KNGlobal::setMainWindow(KNMainWindow *mainWindow)
{
    //Check the main window pointer, once there's a main window, we won't have
    //the second one.
    if(!m_mainWindow)
    {
        //Save the main window pointer.
        m_mainWindow = mainWindow;
    }
}
