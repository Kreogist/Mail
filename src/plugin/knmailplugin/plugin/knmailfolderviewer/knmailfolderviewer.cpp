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

#include "knmailmodel.h"
#include "knmailfolderviewertitle.h"

#include "knmailfolderviewer.h"

#include <QDebug>

KNMailFolderViewer::KNMailFolderViewer(QWidget *parent) :
    KNMailFolderViewerBase(parent),
    m_title(new KNMailFolderViewerTitle(this))
{
    setObjectName("MailFolderViewer");
    //Set properties.
    setAutoFillBackground(true);
    //Register to the theme manager.
    knTheme->registerWidget(this);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add widget to main layout.
    mainLayout->addWidget(m_title);
    mainLayout->addStretch();
}

void KNMailFolderViewer::setFolderModel(KNMailModel *folderModel)
{
    //Check the model.
    if(folderModel==nullptr)
    {
        //Clear the title bar folder name.
        m_title->clear();
        return;
    }
    //Set the folder model title.
    m_title->setFolderName(folderModel->folderName());
}