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

#ifndef KMMIMEMULTIPART_H
#define KMMIMEMULTIPART_H

#include "kmmimebase.h"

/*!
 * \brief The KMMimeMultiPart class provides the ability to describe the multi
 * part of several mime part. This is kind of list container in the mime format.
 */
class KMMimeMultiPart : public KMMimeBase
{
    Q_OBJECT
public:
    enum MultiPartType
    {
        Mixed           = 0,            // RFC 2046, section 5.1.3
        Digest          = 1,            // RFC 2046, section 5.1.5
        Alternative     = 2,            // RFC 2046, section 5.1.4
        Related         = 3,            // RFC 2387
        Report          = 4,            // RFC 6522
        Signed          = 5,            // RFC 1847, section 2.1
        Encrypted       = 6,            // RFC 1847, section 2.2
        MultiPartTypeCount
    };

    /*!
     * \brief Construct a KMMimeMultiPart object.
     * \param parent The parent object pointer.
     */
    explicit KMMimeMultiPart(QObject *parent = 0);

    /*!
     * \brief Add mime part to multi part list.
     * \param part Mime part pointer.
     */
    void addPart(KMMimeBase *part);

    /*!
     * \brief Get the part list of the current multi part.
     * \return Mime part list.
     */
    QList<KMMimeBase *> parts() const;

    /*!
     * \brief Get the multi mime part type.
     * \return Mime type.
     */
    MultiPartType multiType() const;

    /*!
     * \brief Reimplemented from KMMimeBase::toString().
     */
    QString toString();

signals:

public slots:
    /*!
     * \brief Set the multi mime part type.
     * \param multiType  Multi mime part type.
     */
    void setMultiType(const MultiPartType &multiType);

private:
    static QString m_multiTypeName[MultiPartTypeCount];
    QList<KMMimeBase *> m_parts;
    MultiPartType m_multiType;
};

#endif // KMMIMEMULTIPART_H
