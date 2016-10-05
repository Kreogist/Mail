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

#ifndef KNMIMEPART_H
#define KNMIMEPART_H

#include <QMap>
#include <QString>
#include <QByteArray>

/*!
 * \brief The KNMimePart class holds a body part to be used inside a HTTP
 * multipart MIME message. It also provides the port for the multiple mime part.
 * A KNMimePart consists of a header block and a data block, which are separated
 * by each other by two consecutive new lines.
 */
class KNMimePart
{
public:
    /*!
     * \brief Construct a KNMimePart object.
     */
    KNMimePart();
    virtual ~KNMimePart();

    /*!
     * \brief Get the mime part property list map.
     * \return The mime part property list.
     */
    QMap<QString, QString> headerList() const;

    /*!
     * \brief Get MIME header information.
     * \param field The field data.
     * \return The value according to the field data, if no field found, return
     * a null string.
     */
    QString mimeHeader(const QString &field) const;

    /*!
     * \brief Set all the properties at once.
     * \param propertyList The property list map.
     */
    void setMimeHeaderList(const QMap<QString, QString> &headerList);

    /*!
     * \brief Set one mime property via the field name.
     * \param field The mime field name.
     * \param value The mime property value.
     */
    void setMimeHeader(const QString &field, const QString &value);

    /*!
     * \brief Set the mime content body.
     * \param body Raw mime content.
     */
    void setBody(const QByteArray &body);

    /*!
     * \brief Get the body data of raw mime data.
     * \return Raw mime content.
     */
    QByteArray body() const;

    /*!
     * \brief Check whether the mime part has a mime header field.
     * \param field The checking field.
     * \return If contains the header field, return true.
     */
    bool hasMimeHeader(const QString &field) const;

    /*!
     * \brief This function is used to judge whether current mime part is a
     * multi content part.
     * \return For a single mime part, return false.
     */
    virtual bool isMultiPart() const;

    /*!
     * \brief Translate the mime content to string.
     * \return The string content of the mime.
     */
    virtual QString toString();

private:
    inline void headerToString(QString &headerCache, QString &encoding);
    QMap<QString, QString> m_propertyList;
    QByteArray m_content;
};

#endif // KNMIMEPART_H
