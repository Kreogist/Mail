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

#include "knmailglobal.h"

#include "knmailviewer.h"

#define TitleHMargin 31
#define TitleVMargin 26
#define TitleSpacing 5
#define TitleFontSize 18
#define TextFontSize 12

KNMailViewer::KNMailViewer(QWidget *parent) :
    KNMailViewerBase(parent),
    m_subjectText(QString()),
    m_subject(new QLabel(this)),
    m_receiveTime(new QLabel(this)),
    m_senderLabel(new QLabel(this)),
    m_receiverLabel(new QLabel(this)),
    m_ccLabel(new QLabel(this))
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
    m_senderLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    m_receiverLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    m_ccLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);

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
    //Add widget to the layout.
    titleLayout->addWidget(m_subject);
    titleLayout->addWidget(m_receiveTime);
    //Initial the sender, receiver and CC lists.
    QFormLayout *contactLayout=new QFormLayout(mainLayout->widget());
    contactLayout->setContentsMargins(0,0,0,0);
    contactLayout->setSpacing(0);
    titleLayout->addLayout(contactLayout);
    //Configure the data.
    contactLayout->setLabelAlignment(Qt::AlignTop | Qt::AlignRight);
    //Add contact widgets.
    contactLayout->addRow(m_senderLabel);
    contactLayout->addRow(m_receiverLabel);
    contactLayout->addRow(m_ccLabel);
    //Add the last widgets.
    mainLayout->addStretch();
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
}

KNMailViewer::~KNMailViewer()
{
    //Remove the connection between the theme manager and viewer.
    disconnect(knTheme, 0, this, 0);
    //Remove the translation signals.
    disconnect(knI18n, 0, this, 0);
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
}
