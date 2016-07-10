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
#include <QBoxLayout>

#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knunderlinelineedit.h"

#include "knaccountutil.h"
#include "knaccountavatarbutton.h"

#include "knaccountloginpanel.h"

KNAccountLoginPanel::KNAccountLoginPanel(QWidget *parent) :
    KNAccountPanel(parent),
    m_nullAvatar(new KNAccountAvatarButton(this)),
    m_username(new KNUnderLineLineEdit(this)),
    m_password(new KNUnderLineLineEdit(this))
{
    setObjectName("AccountPanel");
    //Set properties.
    setAutoFillBackground(true);
    setFixedHeight(AccountPanelHeight);
    knTheme->registerWidget(this);
    //Configure the null avatar widget.
    m_nullAvatar->setFixedSize(AccountAvatarSize, AccountAvatarSize);
    m_nullAvatar->setAccountAvatar(QPixmap("://public/anonymous.png"));
    //Configure the password widget.
    m_password->setEchoMode(QLineEdit::Password);
    //Initial layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    setLayout(mainLayout);
    //Add panel widget.
    mainLayout->addWidget(m_nullAvatar);
    //Initial the input box layout.
    QBoxLayout *inputLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                           mainLayout->widget());
    mainLayout->addLayout(inputLayout, 1);
    //Add widgets.
    inputLayout->addWidget(m_username);
    inputLayout->addWidget(m_password);
    inputLayout->addStretch();

    //Link with the theme changed signal.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNAccountLoginPanel::onActionThemeChanged);
    onActionThemeChanged();

    //Link retranslator.
    knI18n->link(this, &KNAccountLoginPanel::retranslate);
    retranslate();
}

void KNAccountLoginPanel::retranslate()
{
    m_username->setPlaceholderText(tr("Username"));
    m_password->setPlaceholderText(tr("Password"));
}

void KNAccountLoginPanel::onActionThemeChanged()
{
    //Get the palette.
    QPalette &&lineEditPalette=knTheme->getPalette("AccountPanelLineEdit");
    //Give the palette to line edit.
    m_username->updatePalette(lineEditPalette);
    m_password->updatePalette(lineEditPalette);
}
