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
#include <QBoxLayout>
#include <QLabel>

#include "knlabellineedit.h"
#include "knhwidgetswitcher.h"
#include "knopacityanimebutton.h"

#include "knmailaccountadddialog.h"

#define LineEditWidth 250

KNMailAccountAddDialog::KNMailAccountAddDialog(QWidget *parent) :
    KNMessageBox(parent),
    m_loginHintText(new QLabel(this)),
    m_widgetSwitcher(new KNHWidgetSwitcher(this))
{
    //Set properties.
    setTitleText("Add Mail Account");
    setOkayButtonVisible(false);
    setCancelButtonVisible(true);
    setContentWidget(m_widgetSwitcher);
    //Update size of the widget switcher.
    m_widgetSwitcher->setFixedHeight(194);
    //Construct panel.
    for(int i=0; i<AddAccountPanelsCount; ++i)
    {
        //Construct panel.
        m_panelContainer[i]=new QWidget(this);
        //Add panel to switcher.
        m_widgetSwitcher->addWidget(m_panelContainer[i]);
    }

    //Build Username and Password Panel:
    QBoxLayout *panelLayout=new QBoxLayout(
                QBoxLayout::TopToBottom,
                m_panelContainer[UsernamePasswordPanel]);
    m_panelContainer[UsernamePasswordPanel]->setLayout(panelLayout);
    //Insert widget to panel.
    panelLayout->addStretch();
    KNLabelLineEdit *usernameField=new KNLabelLineEdit(this);
    usernameField->setMinimumLightness(0xC0);
    usernameField->setMediumLightness(0xE0);
    usernameField->setFixedWidth(LineEditWidth);
    usernameField->setLabelIcon(
                QPixmap(":/plugin/mail/account/account_username.png"));
    panelLayout->addWidget(usernameField, 0, Qt::AlignHCenter);
    KNLabelLineEdit *passwordField=new KNLabelLineEdit(this);
    passwordField->setMinimumLightness(0xC0);
    passwordField->setMediumLightness(0xE0);
    passwordField->setEchoMode(QLineEdit::Password);
    passwordField->setFixedWidth(LineEditWidth);
    passwordField->setLabelIcon(
                QPixmap(":/plugin/mail/account/account_password.png"));
    panelLayout->addWidget(passwordField, 0, Qt::AlignHCenter);
    KNOpacityAnimeButton *nextButton=new KNOpacityAnimeButton(this);
    nextButton->hide();
    nextButton->setIcon(QIcon(":/plugin/mail/account/account_next.png"));
    nextButton->setFixedSize(40, 40);
    connect(passwordField, &KNLabelLineEdit::textChanged,
            [=](const QString &password){
        nextButton->setVisible(!password.isEmpty());
    });
    connect(nextButton, &KNOpacityAnimeButton::clicked,
            [=]{
        //Disable the username and password.
        usernameField->setEnabled(false);
        passwordField->setEnabled(false);
        //Hide the next button.
        nextButton->hide();
    });
    panelLayout->addSpacing(12);
    panelLayout->addWidget(nextButton, 0, Qt::AlignHCenter);
    panelLayout->addStretch();
}
