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
#include "kmmailcomponentcontactarea.h"
#include "kmmailcomponentcontactlist.h"
#include "kmmailcontactbutton.h"

#include "kmmailcomponenttitlebar.h"

#include <QDebug>

#define TopBottomMargin 26
#define LeftRightMargin 31
#define TopBottomSpacing 5

KMMailComponentTitleBar::KMMailComponentTitleBar(QWidget *parent) :
    QWidget(parent),
    m_receiveDate(QDate()),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom, this)),
    m_titleLabel(new QLabel(this)),
    m_receiveLabel(new QLabel(this)),
    m_fromLabel(new QLabel(this)),
    m_toLabel(new QLabel(this)),
    m_fromArea(new KMMailComponentContactArea(this)),
    m_toArea(new KMMailComponentContactArea(this)),
    m_fromListWidget(new KMMailComponentContactList(this)),
    m_toListWidget(new KMMailComponentContactList(this))
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
    //Configure the layout.
    m_toListWidget->setEnableFold(true);
    //Update the from and to list.
    connect(m_fromListWidget, &KMMailComponentContactList::expandStateChange,
            this, &KMMailComponentTitleBar::onActionExpandChanged);
    connect(m_toListWidget, &KMMailComponentContactList::expandStateChange,
            this, &KMMailComponentTitleBar::onActionExpandChanged);

    //Configure the main layout.
    m_mainLayout->setContentsMargins(LeftRightMargin,
                                     TopBottomMargin,
                                     LeftRightMargin,
                                     TopBottomMargin);
    m_mainLayout->setSpacing(TopBottomSpacing);
    //Set the layout.
    setLayout(m_mainLayout);
    //Add widget to layout.
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_receiveLabel);
    //Initial the from and receive form.
    QFormLayout *fromToLayout=new QFormLayout(m_mainLayout->widget());
    m_mainLayout->addLayout(fromToLayout);
    //Configure the from/to layout.
    fromToLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    fromToLayout->addRow(m_fromLabel, m_fromListWidget);
    fromToLayout->addRow(m_toLabel, m_toListWidget);

    //Link the theme manager.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KMMailComponentTitleBar::onThemeChanged);
    //Link retranslator.
    knI18n->link(this, &KMMailComponentTitleBar::retranslate);
    retranslate();
}

bool KMMailComponentTitleBar::isExpand() const
{
    return m_toListWidget->isExpaned() || m_fromListWidget->isExpaned();
}

void KMMailComponentTitleBar::setReceiverList(const QStringList &addressList)
{
    //Clear the layout.
    m_toListWidget->clear();
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
        m_toListWidget->addWidget(button);
    }
    //Update the title height.
    updateHeight();
}

void KMMailComponentTitleBar::setSenderList(const QStringList &senderList)
{
    //Clear the layout.
    m_fromListWidget->clear();
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
        m_fromListWidget->addWidget(button);
    }
    //Update the title height.
    updateHeight();
}

void KMMailComponentTitleBar::setTitle(const QString &text)
{
    //Save the title label text.
    m_titleLabel->setText(text);
    //Update the title height.
    updateHeight();
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

void KMMailComponentTitleBar::onActionExpandChanged()
{
    //Update the height.
    updateHeight();
    //Emit the update changed signal.
    emit titleSizeUpdate();
}

void KMMailComponentTitleBar::updateHeight(int targetWidth)
{
    //Update the size.
    setFixedHeight((TopBottomMargin<<1) +
                   m_titleLabel->heightForWidth(
                       (targetWidth==-1?width():targetWidth)-
                       (LeftRightMargin<<1)) +
                   m_receiveLabel->height() +
                   m_fromListWidget->height() +
                   m_toListWidget->height() +
                   TopBottomSpacing * 3);
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
