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

#include <QObject>

/*!
 * \def knMailGlobal
 * A global pointer referring to the unique music global object.
 */

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

signals:

public slots:

private:
    static KNMailGlobal *m_instance;
    explicit KNMailGlobal(QObject *parent = 0);
    KNMailGlobal(const KNMailGlobal &);
    KNMailGlobal(KNMailGlobal &&);
};

#endif // KNMAILGLOBAL_H
