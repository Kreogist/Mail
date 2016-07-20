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
#ifndef KNMAILGLOBAL_H
#define KNMAILGLOBAL_H

#include <QHash>

#include "knmailutil.h"

#include <QObject>

using namespace MailUtil;

/*!
 * \def knMailGlobal
 * A global pointer referring to the unique mail global object.
 */
#define knMailGlobal (KNMailGlobal::instance())

/*!
 * \brief The KNMailGlobal class provides some public or share data between the
 * mail plugin inside. For public structures or shared functions, it will all be
 * used in KNMailUtil class.
 */
class KNMailGlobal : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the mail global class instance.
     * \return The global instance of the KNMailGlobal.
     */
    static KNMailGlobal *instance();

    /*!
     * \brief Initial the mail global instance.
     * \param parent The parent of the instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the default folder name. It will be translate via the
     * different language.
     * \param index The folder index, it will be check before calling. It should
     * be no less than 0 and less than DefaultFolderCount.
     * \return Translated default folder name.
     */
    QString defaultFolderName(int index);

    /*!
     * \brief Get the viewer title text according to the index.
     * \param index Title viewer text index. Must in range of enumeration
     * MailViewerTitleField.
     * \return The title viewer text.
     */
    QString viewerTitleText(int index);

    /*!
     * \brief Get the provider icon.
     * \param index The provider index.
     * \return The icon of the E-mail provider.
     */
    QPixmap providerIcon(const QString &providerName);

signals:

public slots:

private slots:
    void retranslate();

private:
    static KNMailGlobal *m_instance;
    explicit KNMailGlobal(QObject *parent = 0);
    KNMailGlobal(const KNMailGlobal &);
    KNMailGlobal(KNMailGlobal &&);

    QString m_defaultFolderName[DefaultFolderCount],
            m_titleFieldText[MailViewerTitleFieldCount];
    QHash<QString, QPixmap> m_providerIcon;
};

#endif // KNMAILGLOBAL_H
