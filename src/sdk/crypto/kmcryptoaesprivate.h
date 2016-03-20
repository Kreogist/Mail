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

#ifndef KMCRYPTOAESPRIVATE_H
#define KMCRYPTOAESPRIVATE_H

#include <QByteArray>
#include <QObject>

#define N_ROW                   4
#define N_COL                   4
#define N_BLOCK   (N_ROW * N_COL)
#define N_MAX_ROUNDS           14

#define WPOLY   0x011b
#define BPOLY     0x1b
#define DPOLY   0x008d

#define f1(x)   (x)
#define f2(x)   ((x << 1) ^ (((x >> 7) & 1) * WPOLY))
#define f4(x)   ((x << 2) ^ (((x >> 6) & 1) * WPOLY) ^ (((x >> 6) & 2) * WPOLY))
#define f8(x)   ((x << 3) ^ (((x >> 5) & 1) * WPOLY) ^ (((x >> 5) & 2) * WPOLY)\
                          ^ (((x >> 5) & 4) * WPOLY))
#define d2(x)   (((x) >> 1) ^ ((x) & 1 ? DPOLY : 0))

#define f3(x)   (f2(x) ^ x)
#define f9(x)   (f8(x) ^ x)
#define fb(x)   (f8(x) ^ f2(x) ^ x)
#define fd(x)   (f8(x) ^ f4(x) ^ x)
#define fe(x)   (f8(x) ^ f4(x) ^ f2(x))

#define s_box(x)     sbox[(x)]
#define is_box(x)    isbox[(x)]
#define gfm2_sb(x)   gfm2_sbox[(x)]
#define gfm3_sb(x)   gfm3_sbox[(x)]
#define gfm_9(x)     gfmul_9[(x)]
#define gfm_b(x)     gfmul_b[(x)]
#define gfm_d(x)     gfmul_d[(x)]
#define gfm_e(x)     gfmul_e[(x)]

#define block_copy_nn(d, s, l)    memcpy(d, s, l)
#define block_copy(d, s)          memcpy(d, s, N_BLOCK)

#define sb_data(w) {    /* S Box data values */                            \
    w(0x63), w(0x7c), w(0x77), w(0x7b), w(0xf2), w(0x6b), w(0x6f), w(0xc5),\
    w(0x30), w(0x01), w(0x67), w(0x2b), w(0xfe), w(0xd7), w(0xab), w(0x76),\
    w(0xca), w(0x82), w(0xc9), w(0x7d), w(0xfa), w(0x59), w(0x47), w(0xf0),\
    w(0xad), w(0xd4), w(0xa2), w(0xaf), w(0x9c), w(0xa4), w(0x72), w(0xc0),\
    w(0xb7), w(0xfd), w(0x93), w(0x26), w(0x36), w(0x3f), w(0xf7), w(0xcc),\
    w(0x34), w(0xa5), w(0xe5), w(0xf1), w(0x71), w(0xd8), w(0x31), w(0x15),\
    w(0x04), w(0xc7), w(0x23), w(0xc3), w(0x18), w(0x96), w(0x05), w(0x9a),\
    w(0x07), w(0x12), w(0x80), w(0xe2), w(0xeb), w(0x27), w(0xb2), w(0x75),\
    w(0x09), w(0x83), w(0x2c), w(0x1a), w(0x1b), w(0x6e), w(0x5a), w(0xa0),\
    w(0x52), w(0x3b), w(0xd6), w(0xb3), w(0x29), w(0xe3), w(0x2f), w(0x84),\
    w(0x53), w(0xd1), w(0x00), w(0xed), w(0x20), w(0xfc), w(0xb1), w(0x5b),\
    w(0x6a), w(0xcb), w(0xbe), w(0x39), w(0x4a), w(0x4c), w(0x58), w(0xcf),\
    w(0xd0), w(0xef), w(0xaa), w(0xfb), w(0x43), w(0x4d), w(0x33), w(0x85),\
    w(0x45), w(0xf9), w(0x02), w(0x7f), w(0x50), w(0x3c), w(0x9f), w(0xa8),\
    w(0x51), w(0xa3), w(0x40), w(0x8f), w(0x92), w(0x9d), w(0x38), w(0xf5),\
    w(0xbc), w(0xb6), w(0xda), w(0x21), w(0x10), w(0xff), w(0xf3), w(0xd2),\
    w(0xcd), w(0x0c), w(0x13), w(0xec), w(0x5f), w(0x97), w(0x44), w(0x17),\
    w(0xc4), w(0xa7), w(0x7e), w(0x3d), w(0x64), w(0x5d), w(0x19), w(0x73),\
    w(0x60), w(0x81), w(0x4f), w(0xdc), w(0x22), w(0x2a), w(0x90), w(0x88),\
    w(0x46), w(0xee), w(0xb8), w(0x14), w(0xde), w(0x5e), w(0x0b), w(0xdb),\
    w(0xe0), w(0x32), w(0x3a), w(0x0a), w(0x49), w(0x06), w(0x24), w(0x5c),\
    w(0xc2), w(0xd3), w(0xac), w(0x62), w(0x91), w(0x95), w(0xe4), w(0x79),\
    w(0xe7), w(0xc8), w(0x37), w(0x6d), w(0x8d), w(0xd5), w(0x4e), w(0xa9),\
    w(0x6c), w(0x56), w(0xf4), w(0xea), w(0x65), w(0x7a), w(0xae), w(0x08),\
    w(0xba), w(0x78), w(0x25), w(0x2e), w(0x1c), w(0xa6), w(0xb4), w(0xc6),\
    w(0xe8), w(0xdd), w(0x74), w(0x1f), w(0x4b), w(0xbd), w(0x8b), w(0x8a),\
    w(0x70), w(0x3e), w(0xb5), w(0x66), w(0x48), w(0x03), w(0xf6), w(0x0e),\
    w(0x61), w(0x35), w(0x57), w(0xb9), w(0x86), w(0xc1), w(0x1d), w(0x9e),\
    w(0xe1), w(0xf8), w(0x98), w(0x11), w(0x69), w(0xd9), w(0x8e), w(0x94),\
    w(0x9b), w(0x1e), w(0x87), w(0xe9), w(0xce), w(0x55), w(0x28), w(0xdf),\
    w(0x8c), w(0xa1), w(0x89), w(0x0d), w(0xbf), w(0xe6), w(0x42), w(0x68),\
    w(0x41), w(0x99), w(0x2d), w(0x0f), w(0xb0), w(0x54), w(0xbb), w(0x16) }

#define isb_data(w) {   /* inverse S Box data values */                    \
    w(0x52), w(0x09), w(0x6a), w(0xd5), w(0x30), w(0x36), w(0xa5), w(0x38),\
    w(0xbf), w(0x40), w(0xa3), w(0x9e), w(0x81), w(0xf3), w(0xd7), w(0xfb),\
    w(0x7c), w(0xe3), w(0x39), w(0x82), w(0x9b), w(0x2f), w(0xff), w(0x87),\
    w(0x34), w(0x8e), w(0x43), w(0x44), w(0xc4), w(0xde), w(0xe9), w(0xcb),\
    w(0x54), w(0x7b), w(0x94), w(0x32), w(0xa6), w(0xc2), w(0x23), w(0x3d),\
    w(0xee), w(0x4c), w(0x95), w(0x0b), w(0x42), w(0xfa), w(0xc3), w(0x4e),\
    w(0x08), w(0x2e), w(0xa1), w(0x66), w(0x28), w(0xd9), w(0x24), w(0xb2),\
    w(0x76), w(0x5b), w(0xa2), w(0x49), w(0x6d), w(0x8b), w(0xd1), w(0x25),\
    w(0x72), w(0xf8), w(0xf6), w(0x64), w(0x86), w(0x68), w(0x98), w(0x16),\
    w(0xd4), w(0xa4), w(0x5c), w(0xcc), w(0x5d), w(0x65), w(0xb6), w(0x92),\
    w(0x6c), w(0x70), w(0x48), w(0x50), w(0xfd), w(0xed), w(0xb9), w(0xda),\
    w(0x5e), w(0x15), w(0x46), w(0x57), w(0xa7), w(0x8d), w(0x9d), w(0x84),\
    w(0x90), w(0xd8), w(0xab), w(0x00), w(0x8c), w(0xbc), w(0xd3), w(0x0a),\
    w(0xf7), w(0xe4), w(0x58), w(0x05), w(0xb8), w(0xb3), w(0x45), w(0x06),\
    w(0xd0), w(0x2c), w(0x1e), w(0x8f), w(0xca), w(0x3f), w(0x0f), w(0x02),\
    w(0xc1), w(0xaf), w(0xbd), w(0x03), w(0x01), w(0x13), w(0x8a), w(0x6b),\
    w(0x3a), w(0x91), w(0x11), w(0x41), w(0x4f), w(0x67), w(0xdc), w(0xea),\
    w(0x97), w(0xf2), w(0xcf), w(0xce), w(0xf0), w(0xb4), w(0xe6), w(0x73),\
    w(0x96), w(0xac), w(0x74), w(0x22), w(0xe7), w(0xad), w(0x35), w(0x85),\
    w(0xe2), w(0xf9), w(0x37), w(0xe8), w(0x1c), w(0x75), w(0xdf), w(0x6e),\
    w(0x47), w(0xf1), w(0x1a), w(0x71), w(0x1d), w(0x29), w(0xc5), w(0x89),\
    w(0x6f), w(0xb7), w(0x62), w(0x0e), w(0xaa), w(0x18), w(0xbe), w(0x1b),\
    w(0xfc), w(0x56), w(0x3e), w(0x4b), w(0xc6), w(0xd2), w(0x79), w(0x20),\
    w(0x9a), w(0xdb), w(0xc0), w(0xfe), w(0x78), w(0xcd), w(0x5a), w(0xf4),\
    w(0x1f), w(0xdd), w(0xa8), w(0x33), w(0x88), w(0x07), w(0xc7), w(0x31),\
    w(0xb1), w(0x12), w(0x10), w(0x59), w(0x27), w(0x80), w(0xec), w(0x5f),\
    w(0x60), w(0x51), w(0x7f), w(0xa9), w(0x19), w(0xb5), w(0x4a), w(0x0d),\
    w(0x2d), w(0xe5), w(0x7a), w(0x9f), w(0x93), w(0xc9), w(0x9c), w(0xef),\
    w(0xa0), w(0xe0), w(0x3b), w(0x4d), w(0xae), w(0x2a), w(0xf5), w(0xb0),\
    w(0xc8), w(0xeb), w(0xbb), w(0x3c), w(0x83), w(0x53), w(0x99), w(0x61),\
    w(0x17), w(0x2b), w(0x04), w(0x7e), w(0xba), w(0x77), w(0xd6), w(0x26),\
    w(0xe1), w(0x69), w(0x14), w(0x63), w(0x55), w(0x21), w(0x0c), w(0x7d) }

#define mm_data(w) {    /* basic data for forming finite field tables */   \
    w(0x00), w(0x01), w(0x02), w(0x03), w(0x04), w(0x05), w(0x06), w(0x07),\
    w(0x08), w(0x09), w(0x0a), w(0x0b), w(0x0c), w(0x0d), w(0x0e), w(0x0f),\
    w(0x10), w(0x11), w(0x12), w(0x13), w(0x14), w(0x15), w(0x16), w(0x17),\
    w(0x18), w(0x19), w(0x1a), w(0x1b), w(0x1c), w(0x1d), w(0x1e), w(0x1f),\
    w(0x20), w(0x21), w(0x22), w(0x23), w(0x24), w(0x25), w(0x26), w(0x27),\
    w(0x28), w(0x29), w(0x2a), w(0x2b), w(0x2c), w(0x2d), w(0x2e), w(0x2f),\
    w(0x30), w(0x31), w(0x32), w(0x33), w(0x34), w(0x35), w(0x36), w(0x37),\
    w(0x38), w(0x39), w(0x3a), w(0x3b), w(0x3c), w(0x3d), w(0x3e), w(0x3f),\
    w(0x40), w(0x41), w(0x42), w(0x43), w(0x44), w(0x45), w(0x46), w(0x47),\
    w(0x48), w(0x49), w(0x4a), w(0x4b), w(0x4c), w(0x4d), w(0x4e), w(0x4f),\
    w(0x50), w(0x51), w(0x52), w(0x53), w(0x54), w(0x55), w(0x56), w(0x57),\
    w(0x58), w(0x59), w(0x5a), w(0x5b), w(0x5c), w(0x5d), w(0x5e), w(0x5f),\
    w(0x60), w(0x61), w(0x62), w(0x63), w(0x64), w(0x65), w(0x66), w(0x67),\
    w(0x68), w(0x69), w(0x6a), w(0x6b), w(0x6c), w(0x6d), w(0x6e), w(0x6f),\
    w(0x70), w(0x71), w(0x72), w(0x73), w(0x74), w(0x75), w(0x76), w(0x77),\
    w(0x78), w(0x79), w(0x7a), w(0x7b), w(0x7c), w(0x7d), w(0x7e), w(0x7f),\
    w(0x80), w(0x81), w(0x82), w(0x83), w(0x84), w(0x85), w(0x86), w(0x87),\
    w(0x88), w(0x89), w(0x8a), w(0x8b), w(0x8c), w(0x8d), w(0x8e), w(0x8f),\
    w(0x90), w(0x91), w(0x92), w(0x93), w(0x94), w(0x95), w(0x96), w(0x97),\
    w(0x98), w(0x99), w(0x9a), w(0x9b), w(0x9c), w(0x9d), w(0x9e), w(0x9f),\
    w(0xa0), w(0xa1), w(0xa2), w(0xa3), w(0xa4), w(0xa5), w(0xa6), w(0xa7),\
    w(0xa8), w(0xa9), w(0xaa), w(0xab), w(0xac), w(0xad), w(0xae), w(0xaf),\
    w(0xb0), w(0xb1), w(0xb2), w(0xb3), w(0xb4), w(0xb5), w(0xb6), w(0xb7),\
    w(0xb8), w(0xb9), w(0xba), w(0xbb), w(0xbc), w(0xbd), w(0xbe), w(0xbf),\
    w(0xc0), w(0xc1), w(0xc2), w(0xc3), w(0xc4), w(0xc5), w(0xc6), w(0xc7),\
    w(0xc8), w(0xc9), w(0xca), w(0xcb), w(0xcc), w(0xcd), w(0xce), w(0xcf),\
    w(0xd0), w(0xd1), w(0xd2), w(0xd3), w(0xd4), w(0xd5), w(0xd6), w(0xd7),\
    w(0xd8), w(0xd9), w(0xda), w(0xdb), w(0xdc), w(0xdd), w(0xde), w(0xdf),\
    w(0xe0), w(0xe1), w(0xe2), w(0xe3), w(0xe4), w(0xe5), w(0xe6), w(0xe7),\
    w(0xe8), w(0xe9), w(0xea), w(0xeb), w(0xec), w(0xed), w(0xee), w(0xef),\
    w(0xf0), w(0xf1), w(0xf2), w(0xf3), w(0xf4), w(0xf5), w(0xf6), w(0xf7),\
    w(0xf8), w(0xf9), w(0xfa), w(0xfb), w(0xfc), w(0xfd), w(0xfe), w(0xff) }

static const quint8 sbox[256]  =  sb_data(f1);
static const quint8 isbox[256] = isb_data(f1);

static const quint8 gfm2_sbox[256] = sb_data(f2);
static const quint8 gfm3_sbox[256] = sb_data(f3);

static const quint8 gfmul_9[256] = mm_data(f9);
static const quint8 gfmul_b[256] = mm_data(fb);
static const quint8 gfmul_d[256] = mm_data(fd);
static const quint8 gfmul_e[256] = mm_data(fe);

class KMCryptoAesPrivate
{
public:
    struct AesContext
    {
        quint8 ksch[(N_MAX_ROUNDS + 1) * N_BLOCK];
        quint8 round;
    };

    /*!
     * \brief Generate an AES context with specific key.
     * \param aesKey AES-key, the length of the key should be check.
     */
    static AesContext generateContext(const QByteArray &aesKey)
    {
        //Generate the context.
        AesContext context;
        //Generate a unsigned char array, fill the key to unsigned mode.
        quint8 *key=new quint8[aesKey.size()];
        //Translate the AES key to unsigned array.
        toUCharArray(aesKey, key);
        //Set the cipher key for the pre-keyed version
        quint8 cc, rc, hi;
        //Get AES key size.
        int keyLength=aesKey.size();
        //Copy the block.
        block_copy_nn(context.ksch, key, keyLength);
        hi = (keyLength + 28) << 2;
        context.round=(hi >> 4) - 1;
        for(cc=keyLength, rc=1; cc<hi; cc+=4)
        {
            quint8 tt, t0, t1, t2, t3;
            t0 = context.ksch[cc - 4];
            t1 = context.ksch[cc - 3];
            t2 = context.ksch[cc - 2];
            t3 = context.ksch[cc - 1];
            if(cc%keyLength==0)
            {
                tt = t0;
                t0 = s_box(t1) ^ rc;
                t1 = s_box(t2);
                t2 = s_box(t3);
                t3 = s_box(tt);
                rc = f2(rc);
            }
            else if(keyLength>24 && cc % keyLength==16)
            {
                t0 = s_box(t0);
                t1 = s_box(t1);
                t2 = s_box(t2);
                t3 = s_box(t3);
            }
            tt = cc - keyLength;
            context.ksch[cc + 0] = context.ksch[tt + 0] ^ t0;
            context.ksch[cc + 1] = context.ksch[tt + 1] ^ t1;
            context.ksch[cc + 2] = context.ksch[tt + 2] ^ t2;
            context.ksch[cc + 3] = context.ksch[tt + 3] ^ t3;
        }
        //Recover the memory.
        delete[] key;
        //Give back the context.
        return context;
    }

    /*!
     * \brief Encrypt all the data for all the blocks.
     * \param decrypted Raw data.
     * \param encrypted Target output byte array.
     * \param initialVector Initial vector of the encrypted data.
     * \param context AES context from the key.
     * \return If algorithm could encrypted the data successfully, then return
     * true.
     */
    static bool cbcEncrypt(const QByteArray &decrypted,
                           QByteArray &encrypted,
                           const QByteArray &initialVector,
                           const AesContext &context)
    {
        //Check decrypted data size.
        if(decrypted.size()%16!=0)
        {
            return false;
        }
        //Translate the initial vector.
        quint8 *vector=new quint8[initialVector.size()],
                *iv=vector;
        toUCharArray(initialVector, vector);
        //Translate the decrypted data.
        quint8 *source=new quint8[decrypted.size()],
                *in=source;
        toUCharArray(decrypted, source);
        //Prepare the encrypt data.
        quint8 *target=new quint8[decrypted.size()],
                *out=target;
        //Initial the result flag.
        bool result=true;
        //Calculate block counts.
        unsigned long blockCounts=decrypted.size()/16;
        //Loop for all the blocks
        while(blockCounts--)
        {
            //Do xor for the block.
            xorBlock(iv, in);
            //Encrypt the data.
            if(!encryptBlock(iv, iv, context))
            {
                //Set the failed flag.
                result=false;
                //Stop looping.
                break;
            }
            //Copy the data.
            memcpy(out, iv, N_BLOCK);
            //Increase the position of pointer.
            in+=N_BLOCK;out+=N_BLOCK;
        }
        //Write the out data to encrypted byte array.
        encrypted=QByteArray((char *)target, decrypted.size());
        //Recover the memory.
        delete[] vector;
        delete[] source;
        delete[] target;
        //Give back the result.
        return result;
    }

    static bool cbcDecrypt(QByteArray &decrypted,
                           const QByteArray &encrypted,
                           const QByteArray &initialVector,
                           const AesContext &context)
    {
        //Check encrypted data size.
        if(encrypted.size()%16!=0)
        {
            return false;
        }
        //Translate the initial vector.
        quint8 *vector=new quint8[initialVector.size()],
                *iv=vector;
        toUCharArray(initialVector, vector);
        //Translate the decrypted data.
        quint8 *source=new quint8[encrypted.size()],
                *in=source;
        toUCharArray(encrypted, source);
        //Prepare the encrypt data.
        quint8 *target=new quint8[encrypted.size()],
                *out=target;
        //Initial the result flag.
        bool result=true;
        //Calculate block counts.
        unsigned long blockCounts=encrypted.size()/16;
        //Prepare the cache.
        quint8 cache[N_BLOCK];
        //Loop for all the blocks
        while(blockCounts--)
        {
            //Copy the block data.
            memcpy(cache, in, N_BLOCK);
            //Decrypt the data.
            if(!decryptBlock(in, out, context))
            {
                //Set the failed flag.
                result=false;
                //Stop looping.
                break;
            }
            //Do xor for the block.
            xorBlock(out, iv);
            //Copy the data.
            memcpy(iv, cache, N_BLOCK);
            //Increase the position of pointer.
            in+=N_BLOCK;out+=N_BLOCK;
        }
        //Write the out data to encrypted byte array.
        decrypted=QByteArray((char *)target, encrypted.size());
        //Recover the memory.
        delete[] vector;
        delete[] source;
        delete[] target;
        //Give back the result.
        return result;
    }

    /*!
     * \brief Translate a QByteArray to an unsigned 8-bit array.
     * \param src Source QByteArray.
     * \param destination Target array pointer, the array size should be not
     * smaller than QByteArray size.
     */
    static void toUCharArray(const QByteArray &src, quint8 *destination)
    {
        for(int i=0; i<src.size(); ++i)
        {
            //Simply fill the array.
            destination[i]=src.at(i);
        }
    }

private:
    static bool decryptBlock(const quint8 in[N_BLOCK],
                             quint8 out[N_BLOCK],
                             const AesContext &context)
    {
        //Check rnd of context.
        if(context.round)
        {
            //Prepare variables.
            quint8 cache[N_BLOCK];
            //Copy and decrypt the block.
            copyAndKey(cache, in, context.ksch + context.round * N_BLOCK);
            //Inverse shift the sub rows.
            invShiftSubRows(cache);
            //Add round key and inverse mix sub columns.
            for(int r=context.round; --r; )
            {
                //Add round key.
                addRoundKey(cache, context.ksch+r*N_BLOCK);
                //Inverse mix sub columns.
                invMixSubColumns(cache);
            }
            //Coy and decrypt the block, output the result to out array.
            copyAndKey(out, cache, context.ksch);
            //Finished.
            return true;
        }
        //Failed.
        return false;
    }

    static bool encryptBlock(const quint8 in[N_BLOCK],
                             quint8 out[N_BLOCK],
                             const AesContext &context)
    {
        //Check rnd of context.
        if(context.round)
        {
            //Prepare variables.
            quint8 cache[N_BLOCK];
            //Encrypt the block.
            copyAndKey(cache, in, context.ksch);
            //Mix columns and add round key.
            for(int round=1; round<context.round; ++round)
            {
                //Mix column to the block
                mixSubColumns(cache);
                //Add round key.
                addRoundKey(cache, context.ksch+round * N_BLOCK);
            }
            //Shift the sub rows.
            shiftSubRows(cache);
            //Encrypt the block again, write the cache to out pointer.
            copyAndKey(out, cache, context.ksch + context.round * N_BLOCK);
            //Finished.
            return true;
        }
        //Failed.
        return false;
    }

    static void mixSubColumns(quint8 dt[N_BLOCK])
    {
        quint8 st[N_BLOCK];
        block_copy(st, dt);

        dt[ 0]=gfm2_sb(st[0]) ^ gfm3_sb(st[5]) ^ s_box(st[10]) ^ s_box(st[15]);
        dt[ 1]=s_box(st[0]) ^ gfm2_sb(st[5]) ^ gfm3_sb(st[10]) ^ s_box(st[15]);
        dt[ 2]=s_box(st[0]) ^ s_box(st[5]) ^ gfm2_sb(st[10]) ^ gfm3_sb(st[15]);
        dt[ 3]=gfm3_sb(st[0]) ^ s_box(st[5]) ^ s_box(st[10]) ^ gfm2_sb(st[15]);

        dt[ 4]=gfm2_sb(st[4]) ^ gfm3_sb(st[9]) ^ s_box(st[14]) ^ s_box(st[3]);
        dt[ 5]=s_box(st[4]) ^ gfm2_sb(st[9]) ^ gfm3_sb(st[14]) ^ s_box(st[3]);
        dt[ 6]=s_box(st[4]) ^ s_box(st[9]) ^ gfm2_sb(st[14]) ^ gfm3_sb(st[3]);
        dt[ 7]=gfm3_sb(st[4]) ^ s_box(st[9]) ^ s_box(st[14]) ^ gfm2_sb(st[3]);

        dt[ 8]=gfm2_sb(st[8]) ^ gfm3_sb(st[13]) ^ s_box(st[2]) ^ s_box(st[7]);
        dt[ 9]=s_box(st[8]) ^ gfm2_sb(st[13]) ^ gfm3_sb(st[2]) ^ s_box(st[7]);
        dt[10]=s_box(st[8]) ^ s_box(st[13]) ^ gfm2_sb(st[2]) ^ gfm3_sb(st[7]);
        dt[11]=gfm3_sb(st[8]) ^ s_box(st[13]) ^ s_box(st[2]) ^ gfm2_sb(st[7]);

        dt[12]=gfm2_sb(st[12]) ^ gfm3_sb(st[1]) ^ s_box(st[6]) ^ s_box(st[11]);
        dt[13]=s_box(st[12]) ^ gfm2_sb(st[1]) ^ gfm3_sb(st[6]) ^ s_box(st[11]);
        dt[14]=s_box(st[12]) ^ s_box(st[1]) ^ gfm2_sb(st[6]) ^ gfm3_sb(st[11]);
        dt[15]=gfm3_sb(st[12]) ^ s_box(st[1]) ^ s_box(st[6]) ^ gfm2_sb(st[11]);
    }

    static void invMixSubColumns(quint8 dt[N_BLOCK])
    {
        quint8 st[N_BLOCK];
        block_copy(st, dt);

        dt[ 0] = is_box(gfm_e(st[ 0]) ^ gfm_b(st[ 1]) ^
                gfm_d(st[ 2]) ^ gfm_9(st[ 3]));
        dt[ 5] = is_box(gfm_9(st[ 0]) ^ gfm_e(st[ 1]) ^
                gfm_b(st[ 2]) ^ gfm_d(st[ 3]));
        dt[10] = is_box(gfm_d(st[ 0]) ^ gfm_9(st[ 1]) ^
                gfm_e(st[ 2]) ^ gfm_b(st[ 3]));
        dt[15] = is_box(gfm_b(st[ 0]) ^ gfm_d(st[ 1]) ^
                gfm_9(st[ 2]) ^ gfm_e(st[ 3]));

        dt[ 4] = is_box(gfm_e(st[ 4]) ^ gfm_b(st[ 5]) ^
                gfm_d(st[ 6]) ^ gfm_9(st[ 7]));
        dt[ 9] = is_box(gfm_9(st[ 4]) ^ gfm_e(st[ 5]) ^
                gfm_b(st[ 6]) ^ gfm_d(st[ 7]));
        dt[14] = is_box(gfm_d(st[ 4]) ^ gfm_9(st[ 5]) ^
                gfm_e(st[ 6]) ^ gfm_b(st[ 7]));
        dt[ 3] = is_box(gfm_b(st[ 4]) ^ gfm_d(st[ 5]) ^
                gfm_9(st[ 6]) ^ gfm_e(st[ 7]));

        dt[ 8] = is_box(gfm_e(st[ 8]) ^ gfm_b(st[ 9]) ^
                gfm_d(st[10]) ^ gfm_9(st[11]));
        dt[13] = is_box(gfm_9(st[ 8]) ^ gfm_e(st[ 9]) ^
                gfm_b(st[10]) ^ gfm_d(st[11]));
        dt[ 2] = is_box(gfm_d(st[ 8]) ^ gfm_9(st[ 9]) ^
                gfm_e(st[10]) ^ gfm_b(st[11]));
        dt[ 7] = is_box(gfm_b(st[ 8]) ^ gfm_d(st[ 9]) ^
                gfm_9(st[10]) ^ gfm_e(st[11]));

        dt[12] = is_box(gfm_e(st[12]) ^ gfm_b(st[13]) ^
                gfm_d(st[14]) ^ gfm_9(st[15]));
        dt[ 1] = is_box(gfm_9(st[12]) ^ gfm_e(st[13]) ^
                gfm_b(st[14]) ^ gfm_d(st[15]));
        dt[ 6] = is_box(gfm_d(st[12]) ^ gfm_9(st[13]) ^
                gfm_e(st[14]) ^ gfm_b(st[15]));
        dt[11] = is_box(gfm_b(st[12]) ^ gfm_d(st[13]) ^
                gfm_9(st[14]) ^ gfm_e(st[15]));
      }

    static void xorBlock(void *d, const void *s)
    {
        ((quint32 *)d)[0] ^= ((quint32 *)s)[0];
        ((quint32 *)d)[1] ^= ((quint32 *)s)[1];
        ((quint32 *)d)[2] ^= ((quint32 *)s)[2];
        ((quint32 *)d)[3] ^= ((quint32 *)s)[3];
    }

    static void addRoundKey(quint8 d[N_BLOCK], const quint8 k[N_BLOCK])
    {
        xorBlock(d, k);
    }

    static void copyAndKey(void *d, const void *s, const void *k)
    {
        ((quint32 *)d)[0] = ((quint32 *)s)[0] ^ ((quint32 *)k)[0];
        ((quint32 *)d)[1] = ((quint32 *)s)[1] ^ ((quint32 *)k)[1];
        ((quint32 *)d)[2] = ((quint32 *)s)[2] ^ ((quint32 *)k)[2];
        ((quint32 *)d)[3] = ((quint32 *)s)[3] ^ ((quint32 *)k)[3];
    }

    static void shiftSubRows(quint8 st[N_BLOCK])
    {
        quint8 tt;

        st[ 0] = s_box(st[ 0]); st[ 4] = s_box(st[ 4]);
        st[ 8] = s_box(st[ 8]); st[12] = s_box(st[12]);

        tt = st[1]; st[ 1] = s_box(st[ 5]); st[ 5] = s_box(st[ 9]);
        st[ 9] = s_box(st[13]); st[13] = s_box( tt );

        tt = st[2]; st[ 2] = s_box(st[10]); st[10] = s_box( tt );
        tt = st[6]; st[ 6] = s_box(st[14]); st[14] = s_box( tt );

        tt = st[15]; st[15] = s_box(st[11]); st[11] = s_box(st[ 7]);
        st[ 7] = s_box(st[ 3]); st[ 3] = s_box( tt );
    }

    static void invShiftSubRows(quint8 st[N_BLOCK])
    {
        quint8 tt;

        st[ 0] = is_box(st[ 0]); st[ 4] = is_box(st[ 4]);
        st[ 8] = is_box(st[ 8]); st[12] = is_box(st[12]);

        tt = st[13]; st[13] = is_box(st[9]); st[ 9] = is_box(st[5]);
        st[ 5] = is_box(st[1]); st[ 1] = is_box( tt );

        tt = st[2]; st[ 2] = is_box(st[10]); st[10] = is_box( tt );
        tt = st[6]; st[ 6] = is_box(st[14]); st[14] = is_box( tt );

        tt = st[3]; st[ 3] = is_box(st[ 7]); st[ 7] = is_box(st[11]);
        st[11] = is_box(st[15]); st[15] = is_box( tt );
    }
};

#endif // KMCRYPTOAESPRIVATE_H
