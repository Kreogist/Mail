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
#ifndef KNACCOUNTLOGINPANEL_H
#define KNACCOUNTLOGINPANEL_H

#include "knaccountpanel.h"

class KNAccountAvatarButton;
class KNUnderLineLineEdit;
/*!
 * \brief The KNAccountLoginPanel class provides a interface for user to login
 * the Kreogist Account. It will only used for calling the account class
 * functions, itself won't contains and operations about the account information.
 */
class KNAccountLoginPanel : public KNAccountPanel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountLoginPanel widget.
     * \param parent The parent widget.
     */
    explicit KNAccountLoginPanel(QWidget *parent = 0);

signals:

public slots:

private slots:
    void retranslate();
    void onActionThemeChanged();

private:
    KNAccountAvatarButton *m_nullAvatar;
    KNUnderLineLineEdit *m_username, *m_password;
};

#endif // KNACCOUNTLOGINPANEL_H
