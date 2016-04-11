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
#include <QLocale>

#include <QPushButton>

#include "knlocalemanager.h"
#include "knthememanager.h"
#include "kmflowlayout.h"
#include "kmmailcontactbutton.h"

#include "kmmailcomponenttitlebar.h"

KMMailComponentTitleBar::KMMailComponentTitleBar(QWidget *parent) :
    QWidget(parent),
    m_receiveDate(QDate()),
    m_titleLabel(new QLabel(this)),
    m_receiveLabel(new QLabel(this)),
    m_fromLabel(new QLabel(this)),
    m_toLabel(new QLabel(this)),
    m_fromLayout(new KMFlowLayout()),
    m_toLayout(new KMFlowLayout())
{
    //Configrue the title label.
    m_titleLabel->setObjectName("MailComponentTitle");
    knTheme->registerWidget(m_titleLabel);
    m_titleLabel->setWordWrap(true);
    QFont titleFont=m_titleLabel->font();
    titleFont.setPixelSize(18);
    m_titleLabel->setFont(titleFont);
    //Configure the text label.
    m_receiveLabel->setObjectName("MailComponentLabel");
    knTheme->registerWidget(m_receiveLabel);
    //Configure the from and to label.
    m_fromLabel->setObjectName("MailComponentLabel");
    knTheme->registerWidget(m_fromLabel);
    m_toLabel->setObjectName("MailComponentLabel");
    knTheme->registerWidget(m_toLabel);

    //Initial the label.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    //Configure the main layout.
    mainLayout->setContentsMargins(31, 26, 31, 26);
    mainLayout->setSpacing(3);
    //Set the layout.
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_receiveLabel);
    //Initial the from and receive form.
    QFormLayout *fromToLayout=new QFormLayout(mainLayout->widget());
    fromToLayout->setLabelAlignment(Qt::AlignTop | Qt::AlignRight);
    //Configure the layout.
    fromToLayout->addRow(m_fromLabel, m_fromLayout);
    fromToLayout->addRow(m_toLabel, m_toLayout);
    //Add layout to main layout.
    mainLayout->addLayout(fromToLayout);

    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KMMailComponentTitleBar::onThemeChanged);
    //Link retranslator.
    knI18n->link(this, &KMMailComponentTitleBar::retranslate);
    retranslate();
}

void KMMailComponentTitleBar::setReceiverList(const QStringList &addressList)
{
    //Clear the layout.
    m_fromLayout->clearItems();
    //Remove all widget until the last item.
    while(!m_toList.isEmpty())
    {
        //Remove the item.
        m_toList.takeFirst()->deleteLater();
    }
    //Add all the reciver address to list.
    for(int i=0; i<addressList.size(); ++i)
    {
        //Generate the button widget.
        KMMailContactButton *button=generateButton(addressList.at(i));
        //Add button to list.
        m_toList.append(button);
        //Add widget to layout.
        m_toLayout->addWidget(button);
    }
}

void KMMailComponentTitleBar::setSenderList(const QStringList &senderList)
{
    //Clear the layout.
    m_fromLayout->clearItems();
    //Remove all widget until the last item.
    while(!m_fromList.isEmpty())
    {
        //Remove the item.
        m_fromList.takeFirst()->deleteLater();
    }
    //Add all the reciver address to list.
    for(int i=0; i<senderList.size(); ++i)
    {
        //Generate the button widget.
        KMMailContactButton *button=generateButton(senderList.at(i));
        //Add button to list.
        m_fromList.append(button);
        //Add widget to layout.
        m_fromLayout->addWidget(button);
    }
}

void KMMailComponentTitleBar::setTitle(const QString &text)
{
    //Save the title label text.
    m_titleLabel->setText(text);
}

void KMMailComponentTitleBar::setReceiveDate(const QDate &receiveDate)
{
    //Save the date.
    m_receiveDate = receiveDate;
    //Update the label.
    m_receiveLabel->setText(
                m_receiveDate.toString(
                    QLocale::system().dateFormat(QLocale::LongFormat)));
}

void KMMailComponentTitleBar::retranslate()
{
    m_fromLabel->setText(tr("From: "));
    m_toLabel->setText(tr("To: "));
}

void KMMailComponentTitleBar::onThemeChanged()
{
    //Update all the button.
    KMMailContactButton *button;
    //Update all from list.
    foreach(button, m_fromList)
    {
        //Update the button item.
        button->setPalette(knTheme->getPalette("MailComponentButton"));
    }
    //Update all to list.
    foreach(button, m_toList)
    {
        //Update the button item.
        button->setPalette(knTheme->getPalette("MailComponentButton"));
    }
}

inline KMMailContactButton *KMMailComponentTitleBar::generateButton(
        const QString &address)
{
    //Generate the button.
    KMMailContactButton *button=new KMMailContactButton(this);
    //Update the palette.
    button->setPalette(knTheme->getPalette("MailComponentButton"));
    //Update the avatar.
    button->setContactAddress(address);
    //Give back the button.
    return button;
}
