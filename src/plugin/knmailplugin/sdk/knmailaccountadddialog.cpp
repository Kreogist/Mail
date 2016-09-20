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
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <QStringListModel>

#include "knlabellineedit.h"
#include "knhwidgetswitcher.h"
#include "knopacityanimebutton.h"

#include "knmailaccount.h"
#include "knmailaccountmanager.h"

#include "knmailaccountadddialog.h"

#define ConfigureWidthLength 170
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
    m_widgetSwitcher->setFixedHeight(230);
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
    panelLayout->addWidget(new QLabel("Please input your account "
                                      "username and password", this),
                           0,
                           Qt::AlignHCenter);
    panelLayout->addSpacing(12);
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
    KNOpacityAnimeButton *userNextButton=new KNOpacityAnimeButton(this);
    userNextButton->hide();
    userNextButton->setIcon(QIcon(":/plugin/mail/account/account_next.png"));
    userNextButton->setFixedSize(30, 30);
    connect(passwordField, &KNLabelLineEdit::textChanged,
            [=](const QString &password){
        userNextButton->setVisible(!password.isEmpty());
    });
    connect(userNextButton, &KNOpacityAnimeButton::clicked,
            [=]{
        //Disable the username and password.
        usernameField->setEnabled(false);
        passwordField->setEnabled(false);
        //Hide the next button.
        userNextButton->hide();
        //Change panel to configuration panel.
        m_widgetSwitcher->setCurrentIndex(ConfigurationPanel);
    });
    panelLayout->addSpacing(12);
    panelLayout->addWidget(userNextButton, 0, Qt::AlignHCenter);
    panelLayout->addStretch();

    panelLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                               m_panelContainer[ConfigurationPanel]);
    m_panelContainer[ConfigurationPanel]->setLayout(panelLayout);
    panelLayout->addStretch();
    QFormLayout *configureLayout=new QFormLayout(panelLayout->widget());
    KNLabelLineEdit *smtpAddress=new KNLabelLineEdit(this);
    smtpAddress->setFixedWidth(ConfigureWidthLength);
    smtpAddress->setMinimumLightness(0xC0);
    smtpAddress->setMediumLightness(0xE0);
    KNLabelLineEdit *smtpPort=new KNLabelLineEdit(this);
    smtpPort->setFixedWidth(ConfigureWidthLength);
    smtpPort->setMinimumLightness(0xC0);
    smtpPort->setMediumLightness(0xE0);
    QComboBox *smtpProtocol=new QComboBox(this);
    smtpProtocol->setFixedWidth(ConfigureWidthLength);
    QStringList connectType;
    connectType << "Plain" << "SSL 3.0" << "TLS 1.0" << "SSL 3.0/TLS 1.0" << "TLS 1.0 or later";
    QStringListModel *connectionTypeModel=new QStringListModel(this);
    connectionTypeModel->setStringList(connectType);
    smtpProtocol->setModel(connectionTypeModel);
    configureLayout->addRow(tr("SMTP Address"), smtpAddress);
    configureLayout->addRow(tr("SMTP Port"), smtpPort);
    configureLayout->addRow(tr("SMTP Conn Type"), smtpProtocol);
    KNLabelLineEdit *imapAddress=new KNLabelLineEdit(this);
    imapAddress->setFixedWidth(ConfigureWidthLength);
    imapAddress->setMinimumLightness(0xC0);
    imapAddress->setMediumLightness(0xE0);
    KNLabelLineEdit *imapPort=new KNLabelLineEdit(this);
    imapPort->setFixedWidth(ConfigureWidthLength);
    imapPort->setMinimumLightness(0xC0);
    imapPort->setMediumLightness(0xE0);
    QComboBox *imapProtocol=new QComboBox(this);
    imapProtocol->setFixedWidth(ConfigureWidthLength);
    imapProtocol->setModel(connectionTypeModel);
    configureLayout->addRow(tr("IMAP Address"), imapAddress);
    configureLayout->addRow(tr("IMAP Port"), imapPort);
    configureLayout->addRow(tr("IMAP Conn Type"), imapProtocol);
    QBoxLayout *configureCenterLayout=new QBoxLayout(QBoxLayout::LeftToRight);
    configureCenterLayout->addStretch();
    configureCenterLayout->addLayout(configureLayout);
    configureCenterLayout->addStretch();
    KNOpacityAnimeButton *configNextButton=new KNOpacityAnimeButton(this);
    configNextButton->setIcon(QIcon(":/plugin/mail/account/account_next.png"));
    configNextButton->setFixedSize(30, 30);
    connect(configNextButton, &KNOpacityAnimeButton::clicked,
            [=]{
        //Construct an mail account object.
        KNMailAccount *mailAccount=new KNMailAccount(this);
        mailAccount->setUsername(usernameField->text());
        mailAccount->setPassword(passwordField->text());
        mailAccount->setSendProtocolName("smtp");
        mailAccount->setReceiveProtocolName("imap");
        KNMailProtocolConfig sendConfig;
        sendConfig.loginFormat="%1 %2";
        sendConfig.hostName=smtpAddress->text();
        sendConfig.port=smtpPort->text().toUInt();
        getProtocolSetting(smtpProtocol->currentIndex(),
                           sendConfig.socketType,
                           sendConfig.sslVersion);
        sendConfig.idCheck=true;
        mailAccount->setSendConfig(sendConfig);
        KNMailProtocolConfig receiveConfig;
        receiveConfig.loginFormat="%1 %2";
        receiveConfig.hostName=imapAddress->text();
        receiveConfig.port=imapPort->text().toUInt();
        getProtocolSetting(imapProtocol->currentIndex(),
                           receiveConfig.socketType,
                           receiveConfig.sslVersion);
        mailAccount->setReceiveConfig(receiveConfig);
        knMailAccountManager->appendAccount(mailAccount);
        //Move to next.
        m_widgetSwitcher->setCurrentIndex(SuccessPanel);
        //Clear all the content.
        usernameField->setEnabled(true);
        usernameField->clear();
        passwordField->setEnabled(true);
        passwordField->clear();
        smtpAddress->clear();
        smtpPort->clear();
        smtpProtocol->setCurrentIndex(0);
        imapAddress->clear();
        imapPort->clear();
        imapProtocol->setCurrentIndex(0);
        //Hide cancel button.
        setButtonVisible(false, false);
        setButtonVisible(true, true);
    });
    panelLayout->addLayout(configureCenterLayout);
    panelLayout->addSpacing(10);
    panelLayout->addWidget(configNextButton,0,Qt::AlignHCenter);
    panelLayout->addStretch();
}

void KNMailAccountAddDialog::showEvent(QShowEvent *event)
{
    //Do original event.
    KNMessageBox::showEvent(event);
    //Reset the switcher position.
    m_widgetSwitcher->setCurrentIndex(0);
}

void KNMailAccountAddDialog::getProtocolSetting(int index,
                                                MailProtocolSocket &protocol,
                                                QSsl::SslProtocol &type)
{
    switch(index) {
    case 0:
        protocol=SocketTcp;
        type=QSsl::AnyProtocol;
        break;
    case 1:
        protocol=SocketSsl;
        type=QSsl::SslV3;
        break;
    case 2:
        protocol=SocketTls;
        type=QSsl::TlsV1_0;
        break;
    case 3:
        protocol=SocketSsl;
        type=QSsl::TlsV1SslV3;
        break;
    case 4:
        protocol=SocketSsl;
        type=QSsl::TlsV1_0OrLater;
        break;
    default:
        break;
    }
}
