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
//Dependencies.
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include "knutil.h"

QColor KNUtil::parseColor(const QString &data)
{
    switch(data.length())
    {
    case 3:
        //Format should be RGB.
        return QColor(data.mid(0,1).toInt(nullptr, 16)*16-1,
                      data.mid(1,1).toInt(nullptr, 16)*16-1,
                      data.mid(2,1).toInt(nullptr, 16)*16-1);
    case 4:
        //Format should be #RGB.
        return QColor(data.mid(1,1).toInt(nullptr, 16)*16-1,
                      data.mid(2,1).toInt(nullptr, 16)*16-1,
                      data.mid(3,1).toInt(nullptr, 16)*16-1);
    case 6:
        //Format should be RRGGBB.
        return QColor(data.mid(0,2).toInt(nullptr, 16),
                      data.mid(2,2).toInt(nullptr, 16),
                      data.mid(4,2).toInt(nullptr, 16));
    case 7:
        //Format should be #RRGGBB
        return QColor(data.mid(1,2).toInt(nullptr, 16),
                      data.mid(3,2).toInt(nullptr, 16),
                      data.mid(5,2).toInt(nullptr, 16));
    case 8:
        //Format should be RRGGBBAA
        return QColor(data.mid(0,2).toInt(nullptr, 16),
                      data.mid(2,2).toInt(nullptr, 16),
                      data.mid(4,2).toInt(nullptr, 16),
                      data.mid(6,2).toInt(nullptr, 16));
    case 9:
        //Format should be #RRGGBBAA
        return QColor(data.mid(1,2).toInt(nullptr, 16),
                      data.mid(3,2).toInt(nullptr, 16),
                      data.mid(5,2).toInt(nullptr, 16),
                      data.mid(7,2).toInt(nullptr, 16));
    default:
        //Or else return a default color;
        return QColor();
    }
}

QString KNUtil::simplifiedPath(const QString &path)
{
    return QFileInfo(path).absoluteFilePath();
}

QString KNUtil::ensurePathValid(const QString &path)
{
    //Gernerate the file info about the given path.
    QFileInfo detectInfo(path);
    //Check if the directory has alread exist.
    if(detectInfo.isDir() && detectInfo.exists())
    {
        return detectInfo.absoluteFilePath();
    }
    //Check if there's a file named the same as the path.
    if(detectInfo.isFile())
    {
        //Remove the file first.
        if(!QFile::remove(detectInfo.absoluteFilePath()))
        {
            return QString();
        }
    }
    //Generate the folder.
    return QDir().mkpath(detectInfo.absoluteFilePath())?
                detectInfo.absoluteFilePath():QString();
}

bool KNUtil::removeDirectory(const QString &directoryPath)
{
    //Initial the result.
    bool result=true;
    //Get the target dir.
    QDir targetDir(directoryPath);
    //If the directory exist.
    if(targetDir.exists())
    {
        //Get all the data in the list.
        for(auto i : targetDir.entryInfoList(
                QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot,
                QDir::DirsFirst))
        {
            //Check the directory name.
            if(i.fileName()=="." || i.fileName()=="..")
            {
                //Move on.
                continue;
            }
            //Check the item is a dir or a file.
            if(i.isDir())
            {
                //Remove the dir.
                result=removeDirectory(i.absoluteFilePath());
            }
            else
            {
                //Remove the file.
                result=QFile::remove(i.absoluteFilePath());
            }
            //Check the result.
            if(!result)
            {
                //Failed to remove the directory.
                return false;
            }
        }
        //Remove the empty directory.
        result=targetDir.remove(directoryPath);
    }
    //Mission complete, give the result.
    return result;
}
