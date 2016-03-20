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
#include <QUuid>

#include "kmcryptoaesprivate.h"

#include "kmcryptoaes.h"

QByteArray KMCryptoAes::encrypt(QByteArray data, const QByteArray &key)
{
    //Use QUuid class to generate the initialVector (uuid).
    QByteArray &&initialVector=QUuid::createUuid().toRfc4122(),
    //Prepend vector to data, give out the initial vector.
               &&encryptResult=encrypt(data, key, initialVector);
    //Check result.
    return encryptResult.isEmpty()?
                QByteArray():
                encryptResult.prepend(initialVector);
}

QByteArray KMCryptoAes::encrypt(QByteArray data,
                                const QByteArray &key,
                                const QByteArray &initialVector)
{
    //Get and check the key/initial vector size.
    int keySize=key.size();
    if(keySize!=16 // 128-bit
            && keySize!=24 // 192-bit
            && keySize!=32 // 256-bit
            && keySize!=64 // 512-bit
            && initialVector.size()!=16)
    {
        //We cannot do the encryption.
        return QByteArray();
    }
    //Add padding.
    {
        //Calculate the padding.
        int padding=16-(data.size() % 16);
        //Fill the data.
        //Add padding.
        data.append(QByteArray(padding, (char)padding));
    }
    //Get the AES context via key.
    KMCryptoAesPrivate::AesContext context=
            KMCryptoAesPrivate::generateContext(key);
    //Prepare target array.
    QByteArray encryptedData;
    //Encrypt the data with source data.
    return (KMCryptoAesPrivate::cbcEncrypt(data,
                                          encryptedData,
                                          initialVector,
                                          context))?
                encryptedData:QByteArray();
}

QByteArray KMCryptoAes::decrypt(QByteArray data, const QByteArray &key)
{
    //Check data size.
    if(data.size()<16)
    {
        //Use the default encrypt function, the result should be larger than 16.
        return QByteArray();
    }
    //Get initial vector from data.
    QByteArray &&initialVector=data.left(16),
    //Remove the vector from data, decrypt the data.
               &&decryptResult=decrypt(data.remove(0, 16), key, initialVector);
    //Check the reuslt.
    return decryptResult.isEmpty()?QByteArray():decryptResult;
}

QByteArray KMCryptoAes::decrypt(QByteArray data,
                                const QByteArray &key,
                                const QByteArray &initialVector)
{
    //Get and check the key/initial vector size.
    int keySize=key.size();
    if(keySize!=16 // 128-bit
            && keySize!=24 // 192-bit
            && keySize!=32 // 256-bit
            && keySize!=64 // 512-bit
            && initialVector.size()!=16)
    {
        //We cannot do the encryption.
        return QByteArray();
    }
    //Get the AES context via key.
    KMCryptoAesPrivate::AesContext context=
            KMCryptoAesPrivate::generateContext(key);
    //Prepare target array.
    QByteArray decrpytedData;
    //Decrypt the data with encrypted data.
    if(!KMCryptoAesPrivate::cbcDecrypt(decrpytedData,
                                       data,
                                       initialVector,
                                       context))
    {
        //Failed to decrypt data.
        return QByteArray();
    }
    //Remove padding.
    {
        //Get padding.
        int padding=decrpytedData.at(decrpytedData.size()-1);
        //Check out whether the data contains padding.
        if(decrpytedData.size() > padding
                && decrpytedData.at(decrpytedData.size()-padding+1)==padding)
        {
            //It should contains padding, remove it.
            decrpytedData.resize(decrpytedData.size()-padding);
        }
    }
    //Give back the data.
    return decrpytedData;
}
