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

#ifndef KMCRYPTOAES_H
#define KMCRYPTOAES_H

#include <QObject>

/*!
 * \brief The KMCryptoAes class provides the ability to encrypt and decrypt a
 * byte array using AES cryptographic algorithm.
 */
class KMCryptoAes
{
public:
    /*!
     * \brief This is a overload function.\n
     * Encrypt the data with the specific key. It will generate an initial
     * vector using QUuid.
     * \param data The decrypt raw data.
     * \param key AES encryption key, the key should be 16/24/32/64 bytes long(
     * 128/192/256/512 bits)
     * \return The encrption data with the key.
     */
    static QByteArray encrypt(QByteArray data,
                              const QByteArray &key);

    /*!
     * \brief Encrypt the data with the specific key and initial vector.
     * \param data The decrypt raw data.
     * \param key AES encryption key, the key should be 16/24/32/64 bytes long(
     * 128/192/256/512 bits)
     * \param initialVector The initial vector which will be used in the AES.
     * \return The encrption data with the key. If failed, it will return a null
     * byte array.
     */
    static QByteArray encrypt(QByteArray data,
                              const QByteArray &key,
                              const QByteArray &initialVector);

    /*!
     * \brief This is a overload function.\n
     * Decrypt the data. The initial vector will be get from the data.
     * \param data Encrypted data.
     * \param key AES encryption key, the key should be 16/24/32/64 bytes long(
     * 128/192/256/512 bits)
     * \return The decrypted data.
     */
    static QByteArray decrypt(QByteArray data,
                              const QByteArray &key);

    /*!
     * \brief Decrypt the data with specific key and initial vector.
     * \param data Encrypted data.
     * \param key AES encryption key, the key should be 16/24/32/64 bytes long(
     * 128/192/256/512 bits)
     * \param initialVector The initial vector which will be used in the AES.
     * \return The decryption data with the key. If failed, it will return a
     * null byte array.
     */
    static QByteArray decrypt(QByteArray data,
                              const QByteArray &key,
                              const QByteArray &initialVector);
};

#endif // KMCRYPTOAES_H
