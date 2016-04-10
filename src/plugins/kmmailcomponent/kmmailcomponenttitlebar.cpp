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

#include "knlocalemanager.h"
#include "knthememanager.h"

#include "kmmailcomponenttitlebar.h"

KMMailComponentTitleBar::KMMailComponentTitleBar(QWidget *parent) :
    QWidget(parent),
    m_receiveDate(QDate()),
    m_titleLabel(new QLabel(this)),
    m_receiveLabel(new QLabel(this)),
    m_fromLabel(new QLabel(this)),
    m_toLabel(new QLabel(this))
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
    //Configure the layout.
    fromToLayout->addRow(m_fromLabel);
    fromToLayout->addRow(m_toLabel);
    //Add layout to main layout.
    mainLayout->addLayout(fromToLayout);

    //Link retranslator.
    knI18n->link(this, &KMMailComponentTitleBar::retranslate);
    retranslate();
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
