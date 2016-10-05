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
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include "knunderlinelineedit.h"
#include "knlocalemanager.h"
#include "knthememanager.h"

#include "knmailaccountmanager.h"
#include "knmailglobal.h"
#include "knmailaccount.h"
#include "knmailcomposeedit.h"
#include "knmailrotatebutton.h"
#include "knmailsendermanager.h"

#include "mime/knmimepart.h"

#include "knmailcompose.h"

#include <QDebug>

KNMailCompose::KNMailCompose(QWidget *parent) :
    KNMailComposeBase(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_subject(new KNUnderLineLineEdit(this)),
    m_textEditor(new KNMailComposeEdit(this)),
    m_senderLabel(new QLabel(this)),
    m_receiverLabel(new QLabel(this)),
    m_ccLabel(new QLabel(this)),
    m_bccLabel(new QLabel(this)),
    m_attachment(new QLabel(this)),
    m_senderText(new QComboBox(this)),
    m_receiverText(new KNUnderLineLineEdit(this)),
    m_ccText(new KNUnderLineLineEdit(this)),
    m_bccText(new KNUnderLineLineEdit(this)),
    m_sendMail(new QPushButton(this))
{
    setObjectName("MailCompose");
    //Set properties.
    setLayout(m_mainLayout);
    setWindowFlags(Qt::Window);
    //Configure the text edit.
    QFont titleFont=m_subject->font();
    titleFont.setPixelSize(20);
    m_subject->setFont(titleFont);
    //Link the subject.
    connect(m_subject, &KNUnderLineLineEdit::textChanged,
            [=](const QString &subjectText)
    {
        //Update the title.
        setWindowTitle(subjectText.isEmpty()?
                           knMailGlobal->noSubjectText():
                           subjectText);
    });
    //Configure the text editor.
    m_textEditor->setFrameStyle(QFrame::NoFrame);
    //Configure the send button.
    m_sendMail->setIcon(QIcon(":/plugin/mail/composer/send.png"));
    m_sendMail->setText(tr("Send"));
    connect(m_sendMail, &QPushButton::clicked,
            this, &KNMailCompose::onSendClicked);
    connect(knMailAccountManager, &KNMailAccountManager::rowsInserted,
            this, &KNMailCompose::onSenderListSizeChange);
    onSenderListSizeChange();
    //Configure the sender combo.
    m_senderText->setModel(knMailAccountManager);

    //Create subject layout
    QBoxLayout *subjectLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_mainLayout->widget());
    subjectLayout->setContentsMargins(0,0,0,0);
    subjectLayout->setSpacing(0);
    m_mainLayout->addLayout(subjectLayout);
    //Add subject widget.
    subjectLayout->addWidget(m_subject, 1);
    subjectLayout->addSpacing(3);
    subjectLayout->addWidget(m_sendMail);
    //Add button layout.
    QBoxLayout *buttonLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                            m_mainLayout->widget());
    //Add button layout to subject layout.
    subjectLayout->addLayout(buttonLayout);
    //Add contact layouts.
    QFormLayout *contactLayout=new QFormLayout(m_mainLayout->widget());
    contactLayout->setLabelAlignment(Qt::AlignTop | Qt::AlignRight);
    m_mainLayout->addLayout(contactLayout);
    //Add widget to contact layout.
    contactLayout->addRow(m_senderLabel, m_senderText);
    contactLayout->addRow(m_receiverLabel, m_receiverText);
    m_ccLabel->hide();m_ccText->hide();
//    contactLayout->addRow(m_ccLabel, m_ccText);
    m_bccLabel->hide();m_bccText->hide();
//    contactLayout->addRow(m_bccLabel, m_bccText);
    m_attachment->hide();
//    contactLayout->addRow(m_attachment, new QWidget(this));
    //Add to main layout.
    m_mainLayout->addWidget(m_textEditor, 1);
    //Add widget to the main layout.
    //Apply the theme to the panel.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMailCompose::onThemeChanged);
    onThemeChanged();
    //Append the translate handle.
    connect(knI18n, &KNLocaleManager::languageChange,
            this, &KNMailCompose::retranslate);
    retranslate();
}

void KNMailCompose::closeEvent(QCloseEvent *event)
{
    //Emit the signal.
    emit aboutToClose();
    //Do original event.
    KNMailComposeBase::closeEvent(event);
}

void KNMailCompose::retranslate()
{
    //Update the place holder text.
    m_subject->setPlaceholderText(tr("Subject"));
    //Update the labels.
    m_senderLabel->setText(tr("Sender"));
    m_receiverLabel->setText(tr("Receiver"));
    m_ccLabel->setText(tr("C.C."));
    m_bccLabel->setText(tr("B.C.C."));
    m_attachment->setText(tr("Attachment"));
    //Update the title.
    if(m_subject->text().isEmpty())
    {
        //Update the title.
        setWindowTitle(knMailGlobal->noSubjectText());
    }
}

void KNMailCompose::onThemeChanged()
{
    //Update the panel palette.
    setPalette(knTheme->getPalette(objectName()));
    //Get the palette.
    QPalette pal=knTheme->getPalette("MailViewerLabel");
    m_senderLabel->setPalette(pal);
    m_receiverLabel->setPalette(pal);
    m_ccLabel->setPalette(pal);
    m_bccLabel->setPalette(pal);
    m_attachment->setPalette(pal);
    //Get the combo palette.
    pal=knTheme->getPalette("MailViewerCombo");
    qDebug()<<pal.color(QPalette::Highlight);
    m_senderText->setPalette(pal);
}

void KNMailCompose::onSendClicked()
{
    //Check receiver.
    if(m_receiverText->text().isEmpty())
    {
        //Cannot send to no receiver.
        return;
    }
    //Get the content.
    QString mailContent=m_textEditor->document()->toPlainText();
    //Construct a mime part.
    KNMimePart *mimePart=new KNMimePart();
    mimePart->setBody(mailContent.toUtf8());
    //Check the selected account.
    KNMailAccount *account=knMailAccountManager->account(
                m_senderText->currentIndex());
    QString senderName;
    if(!account->displayName().isEmpty())
    {
        //Insert the display name.
        senderName="\""+account->displayName()+"\" ";
    }
    //Add the address.
    senderName.append("<"+account->username()+">");
    mimePart->setMimeHeader("From", senderName);
    mimePart->setMimeHeader("To", "<"+m_receiverText->text()+">");
    mimePart->setMimeHeader("Subject", m_subject->text());
    mimePart->setBody(mailContent.toUtf8());
    qDebug()<<account->sendConfig().hostName<<account->sendConfig().port;
    qDebug()<<account->sendConfig().socketType<<account->sendConfig().sslVersion;
    qDebug()<<account->username()<<account->password();
    //Send the mail.
    knMailSenderManager->sendMail(mimePart, account, m_receiverText->text());
    //Close the composer.
    close();
}

void KNMailCompose::onSenderListSizeChange()
{
    m_sendMail->setEnabled(knMailAccountManager->rowCount()>0);
}
