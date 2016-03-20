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
#include <QFile>
#include <QFileInfo>

#include "kmmimefile.h"

KMMimeFile::KMMimeFile(QObject *parent) :
    KMMimeBase(parent)
{
    //Set properties.
    setContentType("application/octet-stream");
    setEncoding(MimeBase64);
}

QString KMMimeFile::filePath()
{
    return getProperty("filePath", QString());
}

QString KMMimeFile::toString()
{
    //Check whether the file path is empty.
    if(containsProperty("filePath"))
    {
        //Get the file.
        QFile targetFile(filePath());
        //Get the file info.
        QFileInfo targetFileInfo(targetFile);
        //Check target file existance.
        if(!targetFileInfo.exists())
        {
            //Failed to generate data.
            return QString();
        }
        //Update the content name.
        setContentName(targetFileInfo.fileName());
        //Update the content data.
        if(!targetFile.open(QIODevice::ReadOnly))
        {
            //Failed to generate data.
            return QString();
        }
        //Set the content.
        setContent(targetFile.readAll());
    }
    //Give back the content.
    return KMMimeBase::toString();
}

void KMMimeFile::setFilePath(const QString &filePath)
{
    insertProperty("filePath", filePath);
}

