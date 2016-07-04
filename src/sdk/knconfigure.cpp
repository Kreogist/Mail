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
#include <QJsonValue>
#include <QJsonArray>
#include <QApplication>
#include <QFont>

#include "knconfigure.h"

QHash<QString, int> KNConfigure::m_typeList=QHash<QString, int>();

KNConfigure::KNConfigure(QObject *parent) :
    QObject(parent),
    m_dataObject(QJsonObject()),
    m_caption(QString())
{
    //Check and build type list.
    buildTypeList();
}

KNConfigure::KNConfigure(const QJsonObject &data,
                         QObject *parent) :
    QObject(parent),
    m_dataObject(data),
    m_caption(QString())
{
    //Check and build type list.
    buildTypeList();
}

QJsonObject KNConfigure::dataObject()
{
    //Update the data object first.
    for(QHash<QString, KNConfigure *>::const_iterator i=
            m_subObjectTable.constBegin();
        i!=m_subObjectTable.constEnd();
        ++i)
    {
        //Update the object according to the key.
        m_dataObject.insert(i.key(), i.value()->dataObject());
    }
    //Return the new data object.
    return m_dataObject;
}

KNConfigure *KNConfigure::getConfigure(const QString &key)
{
    //Check if we can find the key in the hash table.
    if(m_subObjectTable.contains(key))
    {
        return m_subObjectTable.value(key);
    }
    //Check whether the key can be found in the object.
    //Get the key object or set the empty object.
    QJsonObject keyObject=
            m_dataObject.contains(key)?
                m_dataObject.value(key).toObject():
                QJsonObject();
    //Construct a KNConfigure.
    KNConfigure *subConfigure=new KNConfigure(keyObject, this);
    //Save the configure in the hash table.
    m_subObjectTable.insert(key, subConfigure);
    //Return the sub configure.
    return subConfigure;
}

QVariant KNConfigure::data(const QString &key,
                           const QVariant &defaultValue)
{
    //Find the key in the data object.
    if(m_dataObject.contains(key))
    {
        //Get the data first, if the data is .
        QJsonValue value=m_dataObject.value(key);
        //Check the type of the value.
        switch(value.type())
        {
        //If the value type is bool, double, string and array, translate the
        //data to the specific type and use the default construct function of
        //the QVariant to build the value.
        case QJsonValue::Bool:
            return value.toBool();
        case QJsonValue::Double:
            return value.toDouble();
        case QJsonValue::String:
            return value.toString();
        case QJsonValue::Array:
            return QVariant(value.toArray());
        //If the type of the value is object, we have to find whether the object
        //contains a key value called "Type"
        case QJsonValue::Object:
        {
            QJsonObject valueObject=value.toObject();
            if(valueObject.contains("Type"))
            {
                //Find the type string data in the hash list.
                int objectType=
                        m_typeList.value(valueObject.value("Type").toString(),
                                         -1);
                //If we cannot find the object type(-1), return the value as a
                //object.
                if(objectType==-1)
                {
                    return valueObject;
                }
                //Parse the object according to the type.
                switch(objectType)
                {
                case Font:
                {
                    QFont valueFont=QApplication::font();
                    valueFont.setFamily(valueObject.value("Family").toString());
                    valueFont.setPixelSize(valueObject.value("Size").toDouble());
                    valueFont.setBold(valueObject.value("Bold").toBool());
                    valueFont.setItalic(valueObject.value("Italic").toBool());
                    valueFont.setUnderline(valueObject.value("Underline").toBool());
                    valueFont.setStrikeOut(valueObject.value("Strikeout").toBool());
                    valueFont.setKerning(valueObject.value("Kerning").toBool());
                    return QVariant::fromValue(valueFont);
                }
                }
            }
            return valueObject;
        }
        //The last type of the data is Null and Undefined. Return the default
        //value.
        default:
            return defaultValue;
        }
    }
    //Save the data to the object.
    setData(key, defaultValue);
    //Return the default value if we cannot find the key.
    return defaultValue;
}

void KNConfigure::setData(const QString &key, const QVariant &value)
{
    //Because the QJsonObject can only insert QJsonValue, and the construct
    //function of QJsonValue only have the following types:
    //   bool, QString, array, double, object.
    //So we have to translate some complex type variant to object.
    switch(value.type())
    {
    //For the basic types(double, float, int, bool, QString), we will save them
    //directly.
    case QVariant::Double:
        m_dataObject.insert(key, value.toDouble());
        break;
    case QVariant::String:
        m_dataObject.insert(key, value.toString());
        break;
    case QVariant::Int:
        m_dataObject.insert(key, value.toInt());
        break;
    case QVariant::Bool:
        m_dataObject.insert(key, value.toBool());
        break;
    //For advanced types(like Font), we have to translate them to a object.
    case QVariant::Font:
    {
        //Generate the font object.
        QFont font=value.value<QFont>();
        QJsonObject fontObject;
        fontObject.insert("Type", QString("Font"));
        fontObject.insert("Family", font.family());
        fontObject.insert("Size", font.pixelSize());
        fontObject.insert("Bold", font.bold());
        fontObject.insert("Italic", font.italic());
        fontObject.insert("Underline", font.underline());
        fontObject.insert("Strikeout", font.strikeOut());
        fontObject.insert("Kerning", font.kerning());
        //Insert the font object.
        m_dataObject.insert(key, fontObject);
        break;
    }
    default:
        break;
    }
}

void KNConfigure::setDataObject(const QJsonObject &dataObject)
{
    //Save the data object first.
    m_dataObject=dataObject;
    //Give the new data to the sub object managers.
    for(QHash<QString, KNConfigure *>::iterator i=m_subObjectTable.begin();
        i!=m_subObjectTable.end();
        ++i)
    {
        i.value()->setDataObject(
                    m_dataObject.value(i.key()).toObject(QJsonObject()));
    }
}

inline void KNConfigure::buildTypeList()
{
    //Check whether the type list has been built before.
    if(m_typeList.isEmpty())
    {
        m_typeList.insert("Font", Font);
    }
}
