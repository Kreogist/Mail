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
#ifndef KNMAILACCOUNT_H
#define KNMAILACCOUNT_H

#include "knmailutil.h"

#include <QObject>

using namespace MailUtil;

class KNMailModel;
/*!
 * \brief The KNMailAccount class describe a mail services account, it contains
 * but not limited the following parts:
 *  - Send protocol and its configuration.
 *  - Receive protocol and its configuration.
 *  - Models.
 * Models includes four default system folders: Inbox, Drafts, Sent Items and
 * Trash. They also include all the customer folders.
 */
class KNMailAccount : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailAccount object with given parent object.
     * \param parent The parent object pointer.
     */
    explicit KNMailAccount(QObject *parent = 0);

    /*!
     * \brief Get the send protocol config.
     * \return The send protocol using configuration.
     */
    KNMailProtocolConfig sendConfig() const;

    /*!
     * \brief Get the receive protocol config.
     * \return The receive protocol using configuration.
     */
    KNMailProtocolConfig receiveConfig() const;

    /*!
     * \brief Get the account display name.
     * \return E-mail account display name.
     */
    QString displayName() const;

    /*!
     * \brief Get the E-mail service username.
     * \return The login user name of the current account.
     */
    QString username() const;

    /*!
     * \brief Get the account password.
     * \return The password of the E-mail account.
     */
    QString password() const;

    /*!
     * \brief Get the text display string.
     * \return The display string of the combination from the username and
     * display name.
     */
    QString displayString() const;

    /*!
     * \brief Get the totoal folder count. It will include the system folder.
     * \return The system and customer folder count.
     */
    int folderCount();

    /*!
     * \brief Get name of the specific folder.
     * \param folderIndex The folder index.
     * \return The folder name.
     */
    QString folderName(int folderIndex);

    /*!
     * \brief Get the provider name.
     * \return The service provider name, default is an empty string.
     */
    QString provider() const;

    /*!
     * \brief Get the folder model.
     * \param index The folder index.
     * \return The folder model object pointer.
     */
    KNMailModel *folder(int index);

    /*!
     * \brief Get the sending protocol name. For most of the client, it should
     * be SMTP.
     * \return Sending protocol name, in upper case letter.
     */
    QString sendProtocolName() const;

    /*!
     * \brief Get the receiving protocol name. For most of the client, it should
     * be POP3 or IMAP.
     * \return Receiving protocol name, in upper case letter.
     */
    QString receiveProtocolName() const;

    /*!
     * \brief Get all the custom folders. This will be used for update all the
     * folder information.
     * \return The custom folder list.
     */
    QList<KNMailModel *> customFolders() const;

signals:
    /*!
     * \brief When the count of the folder is changed, this signal will be
     * emitted.
     */
    void folderCountChanged();

public slots:
    /*!
     * \brief Set the sending protocol name
     * \param sendProtocolName The sending protocol name.
     */
    void setSendProtocolName(const QString &sendProtocolName);

    /*!
     * \brief Set the receiving protocol name.
     * \param receiveProtocolName The receiving protocol name.
     */
    void setReceiveProtocolName(const QString &receiveProtocolName);

    /*!
     * \brief Set the provider name.
     * \param provider The E-mail account provider name.
     */
    void setProvider(const QString &provider);

    /*!
     * \brief Set the login username.
     * \param username The account login username.
     */
    void setUsername(const QString &username);

    /*!
     * \brief Set the login plain password.
     * \param password The plain password of the E-mail account.
     */
    void setPassword(const QString &password);

    /*!
     * \brief Set the display name of the current account. It will be used in
     * each mail composed by this client.
     * \param displayName The account display name.
     */
    void setDisplayName(const QString &displayName);

    /*!
     * \brief Set the user send protocol config.
     * \param sendConfig The sending configuration.
     */
    void setSendConfig(const KNMailProtocolConfig &sendConfig);

    /*!
     * \brief Set the user receive protocol config.
     * \param receiveConfig The receiving configuration.
     */
    void setReceiveConfig(const KNMailProtocolConfig &receiveConfig);

    /*!
     * \brief Set the new folder list.
     * \param customFolders Updated custom folder.
     */
    void setCustomFolders(const QList<KNMailModel *> &customFolders);

private slots:

private:
    QList<KNMailModel *> m_customFolders;
    KNMailProtocolConfig m_sendConfig, m_receiveConfig;
    QString m_sendProtocolName, m_receiveProtocolName, m_username, m_password,
            m_displayName, m_provider;
    KNMailModel *m_defaultFolders[DefaultFolderCount];
};

#endif // KNMAILACCOUNT_H
