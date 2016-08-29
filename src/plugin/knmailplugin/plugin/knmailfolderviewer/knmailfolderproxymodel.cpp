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
#include "knmailmodel.h"

#include "knmailfolderproxymodel.h"

KNMailFolderProxyModel::KNMailFolderProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_pageIndex(-1),
    m_pageSize(0),
    m_pageStart(-2),
    m_pageEnd(-2)
{
}

int KNMailFolderProxyModel::pageIndex() const
{
    return m_pageIndex;
}

void KNMailFolderProxyModel::setPageIndex(int pageIndex)
{
    //Save the page size.
    m_pageIndex = pageIndex;
    //Update the indexes.
    updateIndexes();
    //Update all the content.
    setFilterFixedString("");
}

int KNMailFolderProxyModel::pageSize() const
{
    return m_pageSize;
}

void KNMailFolderProxyModel::setPageSize(int pageSize)
{
    //Save the page size.
    m_pageSize = pageSize;
    //Update the indexes.
    updateIndexes();
    //Update all the content.
    setFilterFixedString("");
}

bool KNMailFolderProxyModel::filterAcceptsRow(
        int source_row,
        const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)
    //Check teh source_row is in the range or not.
    return (m_pageStart!=-2 && source_row>m_pageStart && source_row<m_pageEnd);
}

inline void KNMailFolderProxyModel::updateIndexes()
{
    //Reset the page start to invalid.
    m_pageStart=-2;
    m_pageEnd=-2;
    //Check the page index.
    if(m_pageIndex==-1)
    {
        //Finished.
        return;
    }
    //Update the page start.
    if(sourceModel())
    {
        //Get the model size.
        int modelSize=sourceModel()->rowCount();
        //Update the start and end.
        m_pageStart=qMin(m_pageIndex*m_pageSize-1, modelSize);
        m_pageEnd=qMin((m_pageIndex+1)*m_pageSize, modelSize);
        //Check the model needs update or not.
        if(static_cast<KNMailModel *>(sourceModel())->needCache(m_pageStart+1,
                                                                m_pageEnd))
        {
            //Emit the update signal.
            emit requireUpdate(m_pageStart+1, m_pageEnd);
        }
    }
}
