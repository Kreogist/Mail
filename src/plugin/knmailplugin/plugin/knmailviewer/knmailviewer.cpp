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

#include "knthememanager.h"

#include "knmailviewer.h"

#define TitleFontSize 18

KNMailViewer::KNMailViewer(QWidget *parent) :
    KNMailViewerBase(parent),
    m_subject(new QLabel(this))
{
    setObjectName("MailViewer");
    //Set properties.
    setAutoFillBackground(true);
    //Configure labels.
    // Subject line.
    QFont subjectFont=font();
    subjectFont.setPixelSize(TitleFontSize);
    m_subject->setFont(subjectFont);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mainLayout);
    //Add widget to the layout.
    mainLayout->addWidget(m_subject);
    mainLayout->addStretch();
}
