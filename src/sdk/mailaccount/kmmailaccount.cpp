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
#include "kmmaillistmodel.h"

#include "kmmailaccount.h"

KMMailAccount::KMMailAccount(QObject *parent) :
    QObject(parent)
{
    //Build system folder models.
    for(int i=0; i<MailAccountSystemModelCount; ++i)
    {
        //Construct the model.
        m_systemFolder[i]=new KMMailListModel(this);
    }
}

KMMailAccount::~KMMailAccount()
{
    //Recover the memory from the list.
    clearCustomerFolder();
}

QString KMMailAccount::accountProperty(int propertyIndex) const
{
    Q_ASSERT(propertyIndex>-1 && propertyIndex<MailAccountPropertiesCount);
    //Get the property.
    return m_properties[propertyIndex];
}

KMMailListModel *KMMailAccount::systemFolder(int folderIndex)
{
    Q_ASSERT(folderIndex>-1 && folderIndex<MailAccountSystemModelCount);
    //Get the folder model.
    return m_systemFolder[folderIndex];
}

KMMailListModel *KMMailAccount::customFolder(int customerIndex)
{
    Q_ASSERT(customerIndex>-1 && customerIndex<m_customFolder.size());
    //Get the customer folder model.
    return m_customFolder.at(customerIndex);
}

void KMMailAccount::setAccountProperty(int propertyIndex, const QString &value)
{
    Q_ASSERT(propertyIndex>-1 && propertyIndex<MailAccountPropertiesCount);
    //Save the property value.
    m_properties[propertyIndex]=value;
}

void KMMailAccount::clearCustomerFolder()
{
    //Remove all customer folder model.
    qDeleteAll(m_customFolder);
    //Clear the folder list.
    m_customFolder.clear();
}
