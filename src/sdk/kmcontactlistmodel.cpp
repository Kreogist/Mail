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
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileSystemWatcher>
#include <QFileInfo>

#include "kmutil.h"

#include "kmcontactlistmodel.h"

#include <QDebug>

#define MajorVersion (1)
#define MinorVersion (0)

KMContactListModel::KMContactListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_contactList(QJsonArray()),
    m_contactFilePath(QString()),
    m_contactWatcher(new QFileSystemWatcher(this))
{
    //Link the contact watcher signal.
    connect(m_contactWatcher, &QFileSystemWatcher::fileChanged,
            this, &KMContactListModel::onActionContactFileChanged);
}

int KMContactListModel::rowCount(const QModelIndex &parent) const
{
    //Parent is not used in this function.
    Q_UNUSED(parent)
    //Always return list size.
    return m_contactList.size();
}

QVariant KMContactListModel::data(const QModelIndex &index, int role) const
{
    //Get the target object.
    QJsonObject &&targetObject=m_contactList.at(index.row()).toObject();
    //Check the role.
    switch(role)
    {
    case Qt::DisplayRole:
        //Return the full name back.
        return targetObject.value("First Name").toString() + " " +
                targetObject.value("Last Name").toString();
    case Qt::ToolTipRole:
    {
        //Translate the E-mail value to array.
        QJsonArray &&mailArray=targetObject.value("E-mail").toArray();
        //Check array size, if array is not empty, return the first one.
        if(mailArray.isEmpty())
        {
            //Give back an empty string list.
            return QString();
        }
        //Construct an new tooltip.
        QString tooltipText;
        //Or else, show up an multi row tooltip.
        for(int i=0; i<mailArray.size(); ++i)
        {
            //Add mail account.
            if(i>0)
            {
                //Append a return character.
                tooltipText.append('\n');
            }
            //Append the E-mail address.
            tooltipText.append(mailArray.at(i).toString());
        }
        //Give back the tooltip text.
        return tooltipText;
    }
    default:
        //All the other data will be empty.
        return QVariant();
    }
}

QString KMContactListModel::contactFilePath() const
{
    return m_contactFilePath;
}

QJsonObject KMContactListModel::searchEmailAddress(const QString &mailAddress)
{
    //Search in all contact list.
    for(QJsonArray::iterator i=m_contactList.begin();
        i!=m_contactList.end();
        ++i)
    {
        //Check the item.
        QJsonObject &&contactItem=(*i).toObject();
        //Check whether it contains E-mail value.
        if(!contactItem.contains("E-mail"))
        {
            //Search next.
            continue;
        }
        //Find the mail address in the E-mail array.
        if(contactItem.value("E-mail").toArray().contains(mailAddress))
        {
            //Hit the target, give back the item.
            return contactItem;
        }
    }
    //Or else, give a null object.
    return QJsonObject();
}

void KMContactListModel::setContactFilePath(const QString &contactFilePath)
{
    //Check the file path before, this property couldn't be changed when
    //instance is running.
    if(!m_contactFilePath.isEmpty())
    {
        //Cannot changed the file path.
        return;
    }
    //Save the file path.
    m_contactFilePath = contactFilePath;
    //Check file existance.
    if(!QFileInfo::exists(m_contactFilePath))
    {
        //Save contact list first.
        saveContactFile();
    }
    //Add the file path to watcher.
    m_contactWatcher->addPath(m_contactFilePath);
}

void KMContactListModel::loadContactFile()
{
    //Check contect file path has been set or not.
    if(m_contactFilePath.isEmpty())
    {
        //File path has never been set before.
        return;
    }
    //Load the file.
    QFile contactFile(m_contactFilePath);
    //Open the contact file as read only mode.
    if(contactFile.open(QIODevice::ReadOnly))
    {
        //Get the contact object.
        QJsonObject contactObject=
                QJsonDocument::fromJson(contactFile.readAll()).object();
        //Check the version.
        if(contactObject.value("Major").toInt()==MajorVersion &&
                contactObject.value("Minor").toInt()<=MinorVersion)
        {
            //Only update the file when data is at major version.
            //Get the array.
            QJsonArray contactData=contactObject.value("Data").toArray();
            //Check contact data size.
            if(!contactData.isEmpty())
            {
                //Begin insert data.
                beginInsertRows(QModelIndex(), 0, contactData.size()-1);
                //Save the contact data.
                m_contactList=contactData;
                //End insert data.
                endInsertRows();
            }
        }
        //Close the file.
        contactFile.close();
    }
}

void KMContactListModel::saveContactFile()
{
    //Check contect file path has been set or not.
    if(m_contactFilePath.isEmpty() ||
            //Or the folder is not exist.
            KMUtil::ensurePathValid(
                QFileInfo(m_contactFilePath).absolutePath()).isEmpty())
    {
        qDebug()<<"Cannot save the file!";
        //File path is not set before.
        return;
    }
    //Initial the file.
    QFile contactFile(m_contactFilePath);
    //Open the contact file in write only mode.
    if(contactFile.open(QIODevice::WriteOnly))
    {
        //Generate the contact library.
        QJsonObject contactObject;
        //Insert the contact version to the object.
        contactObject.insert("Major", MajorVersion);
        contactObject.insert("Minor", MinorVersion);
        //Insert the contact list to object.
        contactObject.insert("Data", m_contactList);
        //Save the contact list.
        contactFile.write(QJsonDocument(contactObject).toJson(
                              //Use compact to decrease the disk space usage.
                              QJsonDocument::Compact));
        //Close the file.
        contactFile.close();
    }
}

void KMContactListModel::onActionContactFileChanged()
{
    //Reload the contact file
    //Clear up all the data, check contact list size first.
    if(!m_contactList.isEmpty())
    {
        //Remove all rows when data is not empty.
        beginRemoveRows(QModelIndex(), 0, m_contactList.size()-1);
        //Reset the contact list.
        m_contactList=QJsonArray();
        //End clear up all the data.
        endRemoveRows();
    }
    //Reload the data.
    loadContactFile();
}
