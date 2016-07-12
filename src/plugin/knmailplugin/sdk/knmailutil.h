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
#ifndef KNMAILUTIL_H
#define KNMAILUTIL_H

#include <QString>

namespace MailUtil
{
    enum DefaultFolders
    {
        FolderInbox,
        FolderDrafts,
        FolderSentItems,
        FolderTrash,
        DefaultFolderCount
    };

    enum FolderModelColumns
    {
        ColumnSender,
        ColumnFlag,
        ColumnTitle,
        ColumnReceiveDate,
        FolderModelColumnCount
    };

    struct KNMailListItem
    {
        //Meta data.
        QString sender;
        QString senderName;
        QString receiver;
        QString receiverName;
        QString title;
        QString breifContext;
        //File information.
        QString fileName;
    };

    struct KNMailProtocolConfig
    {
        QString type;
        QString protocol;
        QString hostName;
        quint16 port;
    };
}

/*!
 * \brief The KNMailUtil class provides a class and namespace for all the
 * structures which might be used in the mail plugin.\n
 */
class KNMailUtil
{
public:

private:
    KNMailUtil();
    KNMailUtil(const KNMailUtil &);
    KNMailUtil(KNMailUtil &&);
};

#endif // KNMAILUTIL_H
