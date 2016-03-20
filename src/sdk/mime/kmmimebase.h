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

#ifndef KMMIMEBASE_H
#define KMMIMEBASE_H

#include <QMap>

#include <QObject>

/*!
 * \brief The KMMimeBase class is the abstract base class of all the mime part
 * in E-mail data.
 */
class KMMimeBase : public QObject
{
    Q_OBJECT
public:
    enum MimeEncoding
    {
        Mime7Bit,
        Mime8Bit,
        MimeBase64,
        MimeQuotedPrintable
    };

    /*!
     * \brief Construct a KMMimeBase object.
     * \param parent The parent object.
     */
    explicit KMMimeBase(QObject *parent = 0);

    /*!
     * \brief Get the appendix header of the mime part.
     * \return The appendix header part of mime.
     */
    QString appendixHeader() const;

    /*!
     * \brief The content of the mime data.
     * \return The mime data content. The content won't be encoded in the
     * encoding format. This function always return the original data.
     */
    QByteArray content() const;

    /*!
     * \brief Get mime content ID.
     * \return Mime content ID.
     */
    QString contentId() const;

    /*!
     * \brief Get mime content name.
     * \return Mime content name.
     */
    QString contentName() const;

    /*!
     * \brief Get mime content type.
     * \return Mime content type.
     */
    QString contentType() const;

    /*!
     * \brief Get mime content charset.
     * \return Content charset type.
     */
    QString contentCharset() const;

    /*!
     * \brief Get the encoding method of the content.
     * \return Content encoding method.
     */
    MimeEncoding encoding() const;

    /*!
     * \brief Translate the mime part to a readable string.
     * \return Mime part content.
     */
    virtual QString toString();

    /*!
     * \brief Format Base64 encoding data into several lines with the maximum
     * line length.
     * \param content The raw content in Base64 encoding.
     * \param maxLength The maximum line length.
     * \return The formatted text.
     */
    static QString formatBase64(const QString &content,
                                int maxLength=76);

    /*!
     * \brief Format the quoted printable encoding data into several lines with
     * the maximum line length.
     * \param content The raw content in quoted printabled encoding.
     * \param maxLength The maximum line length.
     * \return The formatted text.
     */
    static QString formatQuotedPrintable(const QString &content,
                                         int maxLength=76);

    /*!
     * \brief Get content boundary text.
     * \return Content boundary text.
     */
    QString contentBoundary() const;

    /*!
     * \brief Set the content boundary, this function is prepared for mime multi
     * part.
     * \param contentBoundary Content boundary data.
     */
    void setContentBoundary(const QString &contentBoundary);

    /*!
     * \brief Get property value from mime data.
     * \param valueName The value name of the
     * \param defaultValue
     * \return
     */
    QString getProperty(const QString &valueName,
                        const QString &defaultValue);

    /*!
     * \brief Get whether the mime part contains a specific property.
     * \param valueName Property name.
     * \return If the property contains this value, then return true.
     */
    bool containsProperty(const QString &valueName);

    /*!
     * \brief Insert the property to property value.
     * \param valueName The property name.
     * \param value Property value.
     */
    void insertProperty(const QString &valueName,
                        const QString &value);

signals:

public slots:
    /*!
     * \brief Set the mime part appendix header.
     * \param header Mime part appendix header data.
     */
    void setAppendixHeader(const QString &appendixHeader);

    /*!
     * \brief Set the mime content part.
     * \param content The mime content data.
     */
    void setContent(const QByteArray &content);

    /*!
     * \brief Set the mime content ID.
     * \param contentId Mime content ID.
     */
    void setContentId(const QString &contentId);

    /*!
     * \brief Set the mime content name.
     * \param contentName Mime content name.
     */
    void setContentName(const QString &contentName);

    /*!
     * \brief Set mime content type.
     * \param contentType Mime content type.
     */
    void setContentType(const QString &contentType);

    /*!
     * \brief Set mime content charset type.
     * \param contentCharset Mime content charset type.
     */
    void setContentCharset(const QString &contentCharset);

    /*!
     * \brief Set mime encoding.
     * \param encoding Mime content encoding type.
     */
    void setEncoding(const MimeEncoding &encoding);

private:
    QMap<QString, QString> m_propertyMap;
    QByteArray m_content;
    MimeEncoding m_encoding;
};

#endif // KMMIMEBASE_H
