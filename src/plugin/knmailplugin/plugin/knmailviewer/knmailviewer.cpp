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
#include <QLabel>
#include <QFormLayout>

#include "knthememanager.h"
#include "knlocalemanager.h"
#include "knopacityanimebutton.h"

#include "mime/knmimepart.h"
#include "mime/knmimemultipart.h"
#include "mime/knmimeparser.h"
#include "knmailwebviewerbase.h"
#include "knmailcontactbutton.h"
#include "knmailcontactlist.h"
#include "knmailglobal.h"

#include "knmailviewer.h"

#include <QDebug>

#define TitleHMargin 31
#define TitleVMargin 26
#define TitleSpacing 5
#define TitleFontSize 18
#define TextFontSize 12

KNMailViewer::KNMailViewer(QWidget *parent) :
    KNMailViewerBase(parent),
    m_subjectText(QString()),
    m_filePath(QString()),
    m_subject(new QLabel(this)),
    m_receiveTime(new QLabel(this)),
    m_senderLabel(new QLabel(this)),
    m_receiverLabel(new QLabel(this)),
    m_ccLabel(new QLabel(this)),
    m_senderList(new KNMailContactList(this)),
    m_receiverList(new KNMailContactList(this)),
    m_ccList(new KNMailContactList(this)),
    m_popup(new KNOpacityAnimeButton(this)),
    m_viewer(nullptr),
    m_mailContent(nullptr)
{
    setObjectName("MailViewer");
    //Set properties.
    setAutoFillBackground(true);
    //Configure labels.
    // Subject line.
    QFont subjectFont=font();
    subjectFont.setPixelSize(TitleFontSize);
    m_subject->setFont(subjectFont);
    // text labels.
    QFont textFont=font();
    textFont.setPixelSize(TextFontSize);
    m_receiveTime->setFont(textFont);
    // Text hints.
    m_senderLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_senderLabel->setFixedHeight(ButtonHeight);
    m_receiverLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_receiverLabel->setFixedHeight(ButtonHeight);
    m_ccLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_ccLabel->setFixedHeight(ButtonHeight);
    //Configure the animation.
    m_popup->setFixedSize(16, 16);
    m_popup->setIcon(QIcon("://public/popup.png"));
    //Link the popup signals.
    connect(m_popup, &KNOpacityAnimeButton::clicked,
            [=]{emit requirePopup(m_filePath);});

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Initial the title layout.
    QBoxLayout *titleLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                           mainLayout->widget());
    //Configure the spacing.
    titleLayout->setContentsMargins(TitleHMargin, TitleVMargin,
                                    TitleHMargin, TitleVMargin);
    titleLayout->setSpacing(TitleSpacing);
    mainLayout->addLayout(titleLayout);
    //Build the subject layout.
    QBoxLayout *subjectLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             mainLayout->widget());
    subjectLayout->setContentsMargins(0,0,0,0);
    subjectLayout->setSpacing(0);
    //Add widgets to subject layout.
    subjectLayout->addWidget(m_subject, 1);
    subjectLayout->addWidget(m_popup);
    //Add layouts and widgets to the layout.
    titleLayout->addLayout(subjectLayout);
    titleLayout->addWidget(m_receiveTime);
    //Initial the sender, receiver and CC lists.
    QFormLayout *contactLayout=new QFormLayout(mainLayout->widget());
    contactLayout->setContentsMargins(0,0,0,0);
    contactLayout->setSpacing(5);
    titleLayout->addLayout(contactLayout);
    //Configure the data.
    contactLayout->setLabelAlignment(Qt::AlignTop | Qt::AlignRight);
    //Add contact widgets.
    contactLayout->addRow(m_senderLabel, m_senderList);
    contactLayout->addRow(m_receiverLabel, m_receiverList);
    contactLayout->addRow(m_ccLabel, m_ccList);
    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMailViewer::onThemeChanged);
    onThemeChanged();
    //Link the retranslate.
    connect(knI18n, &KNLocaleManager::languageChange,
            this, &KNMailViewer::retranslate);
    retranslate();

    //Debug.
    m_subjectText="Welcome to TechLauncher for Semester 2, 2016";
    m_receiveTime->setText("July 18, 2016 13:00:00");
    m_senderList->addContact("saki.tojo@ll-anime.com", "Tojo Saki");
    m_receiverList->addContact("honoka.kousaka.longer.honoka@ll-anime.com", "Kousaka Honoka");
    m_receiverList->addContact("kotori.minami@ll-anime.com", "Minami Kotori");
    m_receiverList->addContact("umi.sonoda@ll-anime.com", "Sonoda Umi");
    m_receiverList->addContact("umi.sonoda@ll-anime.com", "Sonoda Umi");
    m_receiverList->addContact("umi.sonoda@ll-anime.com", "Sonoda Umi");
    m_receiverList->addContact("umi.sonoda@ll-anime.com", "Sonoda Umi");
    m_ccList->addContact("eri.ayase@ll-anime.com", "Ayase Eri");
    m_ccList->addContact("nozomi.tojo@ll-anime.com", "Tojo Nozomi");
    m_ccList->addContact("nico.yazawa@ll-anime.com", "Yazawa Nico");
    m_ccList->addContact("nico.yazawa@ll-anime.com", "Yazawa Nico");
    m_ccList->addContact("nico.yazawa@ll-anime.com", "Yazawa Nico");
    m_ccList->addContact("nico.yazawa@ll-anime.com", "Yazawa Nico");
    m_ccList->addContact("nico.yazawa@ll-anime.com", "Yazawa Nico");
    m_ccList->addContact("nico.yazawa@ll-anime.com", "Yazawa Nico");
    m_ccList->addContact("nico.yazawa@ll-anime.com", "Yazawa Nico");
}

KNMailViewer::~KNMailViewer()
{
    //Remove the connection between the theme manager and viewer.
    disconnect(knTheme, 0, this, 0);
    //Remove the translation signals.
    disconnect(knI18n, 0, this, 0);
    //Check the mail content.
    if(m_mailContent)
    {
        //Remove the mail content.
        delete m_mailContent;
    }
}

void KNMailViewer::setWebViewer(KNMailWebViewerBase *viewer)
{
    //Check the viewer.
    if(viewer==nullptr)
    {
        return;
    }
    //Save the pointer.
    m_viewer=viewer;
    //Change the relationship.
    m_viewer->setParent(this);
    //Translate the layout pointer to box layout.
    QBoxLayout *mainLayout=static_cast<QBoxLayout *>(layout());
    //Add widget to main layout.
    mainLayout->addWidget(m_viewer, 1);
    //Update the widget.
    update();
}

void KNMailViewer::setViewerPopup(bool isPopup)
{
    //Update the popup state.
    m_popup->setVisible(!isPopup);
    m_popup->setEnabled(!isPopup);
    //Check the popup state.
    if(isPopup)
    {
        //Set the window flag of current widget.
        setWindowFlags(Qt::Dialog);
        //Complete.
        return;
    }
    //Or else make this a widget.
    setWindowFlags(Qt::Widget);
}

void KNMailViewer::loadMail(const QString &mailPath)
{
    //Parse the sender content.
    m_senderList->clear();
    m_receiverList->clear();
    m_ccList->clear();
    //Check the file path first.
    //! FIXME: Add checking codes here.
    //Save the file path.
    m_filePath=mailPath;
    //Parse the mail file into a mime part.
    m_mailContent=KNMimeParser::parseMime(mailPath);
    //Check the content result.
    if(m_mailContent==nullptr)
    {
        //Mission complete.
        return;
    }
    //Parse the mail content.
    m_subjectText=
            KNMailUtil::parseEncoding(m_mailContent->mimeHeader("subject"));
    //Prepare the contact content.
    QString contactName,
            contactAddress=parseMailAddress(m_mailContent->mimeHeader("from"),
                                            contactName);
    //Update the content.
    m_senderList->addContact(contactAddress, contactName);
}

void KNMailViewer::resizeEvent(QResizeEvent *event)
{
    //Resize the viewer first.
    KNMailViewerBase::resizeEvent(event);
    //Update the subject text.
    m_subject->setText(m_subject->fontMetrics().elidedText(m_subjectText,
                                                           Qt::ElideRight,
                                                           m_subject->width()));
}

void KNMailViewer::closeEvent(QCloseEvent *event)
{
    //Emit the close signal if the popup button is hidden.
    if(!m_popup->isVisible())
    {
        //This means it is a popup viewer.
        emit requireClose(m_filePath);
        //Delete this widget later.
        deleteLater();
    }
    //Close the window.
    KNMailViewerBase::closeEvent(event);
}

void KNMailViewer::retranslate()
{
    //Update the text,
    m_receiverLabel->setText(knMailGlobal->viewerTitleText(FieldReceive));
    m_senderLabel->setText(knMailGlobal->viewerTitleText(FieldFrom));
    m_ccLabel->setText(knMailGlobal->viewerTitleText(FieldCarbonCopy));
}

void KNMailViewer::onThemeChanged()
{
    //Update the palette.
    setPalette(knTheme->getPalette(objectName()));
    //Get the label palette.
    const QPalette &labelPal=knTheme->getPalette("MailViewerLabel");
    m_receiveTime->setPalette(labelPal);
    m_receiverLabel->setPalette(labelPal);
    m_senderLabel->setPalette(labelPal);
    m_ccLabel->setPalette(labelPal);
    //Get the button palette.
    const QPalette &buttonPal=knTheme->getPalette("MailViewerButton");
    m_receiverList->setContactPalette(buttonPal);
    m_senderList->setContactPalette(buttonPal);
    m_ccList->setContactPalette(buttonPal);
}

inline QString KNMailViewer::parseMailAddress(const QString &rawData,
                                              QString &addressName)
{
    //Search the content of '<' in the raw data.
    int startPosition=rawData.indexOf('<');
    //Check thte start position.
    if(startPosition==-1)
    {
        //No result find.
        return QString();
    }
    //Find the end position for '>'.
    int endPosition=rawData.indexOf('>');
    //Check the end position.
    if(endPosition==-1)
    {
        //No result find.
        return QString();
    }
    //Parse the address name.
    addressName=rawData.left(startPosition).simplified();
    //Give back the email address.
    return rawData.mid(startPosition+1, endPosition-startPosition-1);
}
