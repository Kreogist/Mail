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
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFontDatabase>
#include <QJsonDocument>
#include <QJsonObject>

#include "knfontmanager.h"

#include <QDebug>

KNFontManager *KNFontManager::m_instance=nullptr;

KNFontManager *KNFontManager::instance()
{
    return m_instance;
}

void KNFontManager::initial(QObject *parent)
{
    if(m_instance==nullptr)
    {
        m_instance=new KNFontManager(parent);
    }
}

KNFontManager::KNFontManager(QObject *parent) :
    QObject(parent)
{
    ;
}

void KNFontManager::loadCustomFontFolder(const QString &folderPath)
{
    QDir fontDir(folderPath);
    //Check the folder is exsist or not, if not exsist, create one.
    if(fontDir.exists())
    {
        //Get all the files, load all the fonts.
        QFileInfoList fontFiles=fontDir.entryInfoList();
        for(QFileInfoList::const_iterator i=fontFiles.constBegin();
            i!=fontFiles.constEnd();
            ++i)
        {
            //Ignore the dot(.) and dot-dot(..).
            if((*i).fileName()=="." || (*i).fileName()=="..")
            {
                continue;
            }
            //If the type of current file is File, then try to load the font.
            if((*i).isFile())
            {
                loadCustomFont(*i);
            }
        }
    }
    else
    {
        fontDir.mkpath(folderPath);
    }
}

void KNFontManager::loadCustomFont(const QString &filePath)
{
    //Add file to application font.
    QFontDatabase::addApplicationFont(filePath);
}

void KNFontManager::loadDefaultFont()
{
    //Get the font configuration.
    QJsonObject fontConfig;
    {
        //Get the default font configuration file.
        QFile defaultFontFile(":/public/default_font.json");
        //Open the file in readonly mode.
        if(defaultFontFile.open(QIODevice::ReadOnly))
        {
            //Save the json data.
            fontConfig=QJsonDocument::fromJson(
                        defaultFontFile.readAll()).object();
            //Close the file.
            defaultFontFile.close();
        }
    }
    //Set the global font.
    setGlobalFont(fontConfig.value("FamilyName").toString());
}

void KNFontManager::setGlobalFont(const QString &fontName,
                                  const qreal &pixelSize)
{
    //Ignore the invalid request.
    if(fontName.isEmpty())
    {
        return;
    }
    //Generate the font from the application font.
    QFont globalFont=QApplication::font();
    globalFont.setFamily(fontName);
    globalFont.setPixelSize(pixelSize);
    //Set the font.
    QApplication::setFont(globalFont);
}
