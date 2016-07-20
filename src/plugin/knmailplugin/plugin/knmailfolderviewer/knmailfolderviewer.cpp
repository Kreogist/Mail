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
#include <QTimeLine>

#include "knthememanager.h"

#include "knmailmodel.h"
#include "knmailfolderviewertitle.h"
#include "knmailfoldertreeview.h"
#include "knmailviewerbase.h"

#include "knmailfolderviewer.h"

#define ViewerLeftSpacing 150

#include <QDebug>

KNMailFolderViewer::KNMailFolderViewer(QWidget *parent) :
    KNMailFolderViewerBase(parent),
    m_title(new KNMailFolderViewerTitle(this)),
    m_folderView(new KNMailFolderTreeView(this)),
    m_shadowLayer(new QWidget(this)),
    m_viewer(nullptr)
{
    setObjectName("MailFolderViewer");
    //Set properties.
    setAutoFillBackground(true);
    //Register to the theme manager.
    knTheme->registerWidget(this);
    //Configure the folder view.
    m_folderView->setObjectName("");
    //Configure the shadow layer.
    m_shadowLayer->hide();
    m_shadowLayer->setAutoFillBackground(true);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
    //Add widget to main layout.
    mainLayout->addWidget(m_title);
    mainLayout->addWidget(m_folderView, 1);
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
    m_folderView->setModel(folderModel);
}

void KNMailFolderViewer::setViewer(KNMailViewerBase *viewer)
{
    //Save the viewer base pointer.
    m_viewer=viewer;
    //Check the viewer pointer.
    if(!m_viewer)
    {
        //Ignore the viewer.
        return;
    }
    //Change owner relationship.
    m_viewer->setParent(this);
    //Update the viewer position and size.
    updateViewerPos();
}

void KNMailFolderViewer::resizeEvent(QResizeEvent *event)
{
    //Resize the folder viewer first.
    KNMailFolderViewerBase::resizeEvent(event);
    //Resize the viewer.
    if(m_viewer && m_viewer->isVisible())
    {
        //Update the viewer position and size.
        updateViewerPos();
    }
}

void KNMailFolderViewer::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KNMailFolderViewerBase::resizeEvent(event);
}

inline void KNMailFolderViewer::updateViewerPos()
{
    //Update the viewer size.
    m_viewer->setGeometry(ViewerLeftSpacing,
                          0,
                          width() - ViewerLeftSpacing,
                          height());
}
