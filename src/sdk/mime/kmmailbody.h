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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KMMAILBODY_H
#define KMMAILBODY_H

#include "kmmimemultipart.h"

#include <QObject>

/*!
 * \brief The KMMailBody class describes a E-mail and it could parse into a
 * standard string for sender protocal to send.\n
 * All the sender could use this object to directly send an E-mail.
 */
class KMMailBody : public QObject
{
    Q_OBJECT
public:
    enum MailElement
    {
        ElementSender,
        ElementSenderName,
        ElementSubject,
        MailElementCount
    };

    enum MailContact
    {
        Receiver,
        CarbonCopy,
        BlindCarbonCopy,
        MailContactCount
    };

    /*!
     * \brief Construct a KMMailBody object.
     * \param parent The parent object pointer.
     */
    explicit KMMailBody(QObject *parent = 0);

    /*!
     * \brief Get mail element, element index should be one of the enumerate in
     * MailElement. Assert check is included in this function.
     * \param elementIndex Element index.
     * \return Element text data.
     */
    QString element(int elementIndex) const;

    /*!
     * \brief Set the mail element. Element index should be one of the enumerate
     * in MailElement enumeration. Assert check is included in this function.
     * \param elementIndex Element index.
     * \param elementData Element text data.
     */
    void setElement(int elementIndex, const QString &elementData);

    /*!
     * \brief Get the contact info of the current mail. The contact index should
     * be one of the enumerate of MailContact, Assert check is included in this
     * function.
     * \param contactIndex Element index.
     * \param mailAddress All of the E-mail address of this contact property.
     * \param name The name part of the E-mail address, the position of name in
     * this array is the same as the corresponding E-mail address in the
     * mailAddress array.
     */
    void contactInfo(int contactIndex,
                     QStringList &mailAddress,
                     QStringList &name);

    /*!
     * \brief Set the contact info of the current mail. The contact index should
     * be one of the enumerate of MailContact, Assert check is included in this
     * function.
     * \param contactIndex Element index.
     * \param mailAddress All of the E-mail address of this contact property.
     * \param name The name part of the E-mail address, the position of name in
     * this array is the same as the corresponding E-mail address in the
     * mailAddress array. The size of mailAddress and name should be the same.
     * Assert will check this.
     */
    void setContactInfo(int contactIndex,
                        const QStringList &mailAddress,
                        const QStringList &name);

    /*!
     * \brief Translate the total mail data to readable string. The data could
     * be save as a eml format file and read by most of the mainstream mail
     * client.
     * \return EML format E-mail content.
     */
    QString toString();

    /*!
     * \brief Load E-mail data from string.
     * \param rawData Raw E-mail data from string.
     */
    void fromString(const QString &rawData);

    /*!
     * \brief Get the content part pointer.
     * \return The content part pointer. If the content is never been set, it
     * will return nullptr.
     */
    KMMimeMultiPart *contentPart() const;

signals:

public slots:
    /*!
     * \brief Set the content part to the mail body, it will change the parent
     * of the multi part to mail body.
     * \param contentPart The content part pointer.
     */
    void setContentPart(KMMimeMultiPart *contentPart);

private:
    inline void appendContact(QString &target, int contactIndex);
    inline void appendEncoding(QString &target,
                                  const QString &data);
    bool parseContact(const QString &data, QString &address, QString &name);
    QStringList m_contact[MailContactCount], m_contactName[MailContactCount];
    QString m_element[MailElementCount];
    KMMimeMultiPart *m_contentPart;
    KMMimeBase::MimeEncoding m_encoding;
};

#endif // KMMAILBODY_H
