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

#include <QSsl>
#include <QString>

namespace MailUtil
{
    enum DefaultFolders
    {
        FolderInbox,
        FolderDrafts,
        FolderSentItems,
        FolderJunk,
        FolderTrash,
        DefaultFolderCount
    };

    enum MailViewerTitleField
    {
        FieldFrom,
        FieldReceive,
        FieldCarbonCopy,
        MailViewerTitleFieldCount
    };

    enum FolderModelColumns
    {
        ColumnSender,
        ColumnFlag,
        ColumnTitle,
        FolderModelColumnCount
    };

    enum MailItemRole
    {
        MailPathRole = Qt::UserRole + 1,
        MailCachedRole
    };

    enum MailProtocolSocket
    {
        SocketTcp,
        SocketTls,
        SocketSsl
    };

    enum MailProtocolError
    {
        ConnectionFailed,
        ConnectionTimeout,
        ResponseTimeout,
        SendDataTimeout,
        LoginUsernameError,
        LoginPasswordError
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
        int uid;
        //Flags.
        bool cached;
        KNMailListItem() :
            uid(-1),
            cached(false)
        {
        }
    };

    struct KNMailProtocolConfig
    {
        QString loginFormat;
        QString hostName;
        quint16 port;
        QSsl::SslProtocol sslVersion;
        MailProtocolSocket socketType;
        bool idCheck;
        KNMailProtocolConfig() :
            port(0),
            sslVersion(QSsl::SecureProtocols),
            socketType(SocketTcp),
            idCheck(false)
        {
        }
    };
}

/*!
 * \brief The KNMailUtil class provides a class and namespace for all the
 * structures which might be used in the mail plugin.\n
 */
class KNMailUtil
{
public:
    /*!
     * \brief Translate a string from UTF-7 encoding.
     * \param data The UTF-7 data.
     * \return Translated UTF-8 format string.
     */
    static QString fromUtf7(const QByteArray &data);

    /*!
     * \brief Parse encoding part of the subject, receiver and sender.
     * \param data The raw data.
     * \return The decoding data from the source.
     */
    static QString parseEncoding(QString data);

    /*!
     * \brief Parse E-mail address from the text with format: name <address>
     * \param rawData The raw data of the text.
     * \param addressName The variables which will holds the address name.
     * \return If it parse the data successfully, return the address of the
     * result, or else an empty string.
     */
    static QString parseMailAddress(const QString &rawData,
                                    QString &addressName);

    /*!
     * \brief Parse the encoding content.
     * \param content The encoding content, it might be in 7-bit, 8-bit, BASE64
     * or quote printable.
     * \param encoding Encoding format.
     * \param decodedContent The decode content cache.
     */
    static void parseContent(const QByteArray &content,
                             const QString &encoding,
                             QByteArray &decodedContent);

    /*!
     * \brief Encode one content.
     * \param content The raw content data.
     * \param encoding The encoding format.
     * \param encodedContent The result content.
     */
    static void encodeContent(const QByteArray &content,
                              const QString &encoding,
                              QString &encodedContent);

private:
    static QString parseEncodingPart(const QString &data);
    KNMailUtil();
    KNMailUtil(const KNMailUtil &);
    KNMailUtil(KNMailUtil &&);
};

#endif // KNMAILUTIL_H
