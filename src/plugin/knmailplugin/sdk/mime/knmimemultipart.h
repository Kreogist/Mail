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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMIMEMULTIPART_H
#define KNMIMEMULTIPART_H

#include "knmimepart.h"

/*!
 * \brief The KNMimeMultiPart class resembles a MIME multipart message to be
 * sent over HTTP.\n
 * A multipart message consists of an arbitrary number of body parts, which are
 * separated by a unique boundary.\n
 * You can't directly set and get body content to a MIME multipart.
 */
class KNMimeMultiPart : public KNMimePart
{
public:
    /*!
     * \brief Consruct a KNMimeMultiPart object.
     */
    KNMimeMultiPart();
    ~KNMimeMultiPart();

    /*!
     * \brief Appends mime part to this multipart. Multipart will take the
     * ownership of the multipart.
     * \param mimePart The mime part.
     */
    void append(KNMimePart *mimePart);

    /*!
     * \brief Reimplemented from KNMimePart::isMultiPart().
     */
    bool isMultiPart() const Q_DECL_OVERRIDE;

    /*!
     * \brief Get the mime part inside the multi part.
     * \param i The mime part index.
     * \return The mime part pointer.
     */
    KNMimePart *mimePart(int i) const;

    /*!
     * \brief Get the mime part count of the multi part.
     * \return The amount of the holding mime parts.
     */
    int mimePartCount() const;

private:
    QByteArray body() const;
    void setBody(const QByteArray &body);
    QList<KNMimePart *> m_mimeParts;
};

#endif // KNMIMEMULTIPART_H
