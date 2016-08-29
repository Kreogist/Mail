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
#include "knclickablelabel.h"

#include "knmailaccount.h"
#include "knmailmodel.h"
#include "knmailfolderviewertitle.h"
#include "knmailfoldertreeview.h"
#include "knmailviewerbase.h"

#include "knmailfolderviewer.h"

#define ViewerLeftSpacing 150
#define MaximumShadowDepth 150.0

#include <QDebug>

KNMailFolderViewer::KNMailFolderViewer(QWidget *parent) :
    KNMailFolderViewerBase(parent),
    m_title(new KNMailFolderViewerTitle(this)),
    m_folderView(new KNMailFolderTreeView(this)),
    m_shadowLayer(new KNClickableLabel(this)),
    m_viewer(nullptr),
    m_viewerAnime(new QTimeLine(200, this)),
    m_isAnimeShown(true)
{
    setObjectName("MailFolderViewer");
    //Set properties.
    setAutoFillBackground(true);
    //Register to the theme manager.
    knTheme->registerWidget(this);
    //Configure the folder view.
    m_folderView->setObjectName("");
    connect(m_folderView, &KNMailFolderTreeView::clicked,
            this, &KNMailFolderViewer::onSelectionChange);
    //Configure the shadow layer.
    m_shadowLayer->hide();
    m_shadowLayer->setAutoFillBackground(true);
    updateShadowDarkness(0.0);
    connect(m_shadowLayer, &KNClickableLabel::clicked,
            [=]
    {
        //Check whether the time line is running.
        if(m_viewerAnime->state()!=QTimeLine::Running)
        {
            //Start the animation to the right most.
            startAnimeViewer(width());
        }
    });
    //Initial the viewer anime.
    m_viewerAnime->setUpdateInterval(16);
    m_viewerAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_viewerAnime, &QTimeLine::frameChanged,
            [=](int frame)
    {
        //Move the viewer.
        m_viewer->move(frame, 0);
        //Update shadow layer.
        updateShadowDarkness(m_isAnimeShown ?
                                 m_viewerAnime->currentValue():
                                 (1.0-m_viewerAnime->currentValue()));
    });

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
    //Clear the selection model connection.
    if(m_folderView->selectionModel())
    {
        //Remove the connection.
        disconnect(m_folderView->selectionModel(), 0, 0, 0);
    }
    //Check the model.
    if(folderModel==nullptr)
    {
        //Clear the title bar folder name.
        m_title->clear();
    }
    else
    {
        //Set the folder model title.
        m_title->setFolderName(folderModel->folderName());
    }
    //Update the folder model
    m_folderView->setModel(folderModel);
    //Check the selection model.
    if(m_folderView->selectionModel())
    {
        //Link the selection model selected signal to show the viewer.
        connect(m_folderView->selectionModel(),
                &QItemSelectionModel::currentChanged,
                this, &KNMailFolderViewer::onSelectionChange);
    }
    //Check the viewer.
    if(m_folderView->isVisible())
    {
        //Hide the viewer.
        hideViewer();
    }
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
    m_viewer->loadMail("/Users/saki/Documents/Kreogist/Mail/Accounts/u5870415@uds.anu.edu.au/Inbox/5.emlx");
    //Link the viewer.
    connect(m_viewer, &KNMailViewerBase::requirePopup,
            this, &KNMailFolderViewer::hideViewer);
    //Change owner relationship.
    m_viewer->setParent(this);
    //Hide the viewer at default.
    m_viewer->hide();
    //Update the viewer position and size.
    updateViewerSize();
}

void KNMailFolderViewer::hideViewer()
{
    //Start the hiddien animation.
    startAnimeViewer(width());
}

void KNMailFolderViewer::resizeEvent(QResizeEvent *event)
{
    //Resize the folder viewer first.
    KNMailFolderViewerBase::resizeEvent(event);
    //Update the shadow size.
    m_shadowLayer->resize(size());
    //Update the header size.
    ;
    //Resize the viewer.
    if(m_viewer)
    {
        //Update the viewer position and size.
        updateViewerSize();
        //Check the animation state.
        if(m_viewerAnime->state()==QTimeLine::Running)
        {
            //Update the frame according to the flag.
            if(m_isAnimeShown)
            {
                //Update the start position.
                m_viewerAnime->setStartFrame(width());
            }
            else
            {
                //Update the end position.
                m_viewerAnime->setEndFrame(width());
            }
        }
    }
}

void KNMailFolderViewer::onSelectionChange(const QModelIndex &current)
{
    Q_UNUSED(current)
    //Check viewer state.
    if(!m_viewer->isVisible())
    {
        //Show the viewer.
        m_viewer->show();
        m_shadowLayer->show();
        //Get the model parent, it should be the account.
        KNMailModel *currentModel=(KNMailModel *)(current.model());
        KNMailAccount *mailAccount=
                static_cast<KNMailAccount *>(currentModel->parent());
        qDebug()<<mailAccount->accountDirectoryPath()+"/"+currentModel->folderName()+"/"+current.data(MailPathRole).toString()+".eml";
        m_viewer->loadMail(mailAccount->accountDirectoryPath()+"/"+currentModel->folderName()+"/"+current.data(MailPathRole).toString()+".eml");
        //Start the animation.
        startAnimeViewer(MaximumShadowDepth);
    }
}

inline void KNMailFolderViewer::updateViewerSize()
{
    //Update the viewer size.
    m_viewer->resize(width() - ViewerLeftSpacing, height());
}

inline void KNMailFolderViewer::updateShadowDarkness(const qreal &alpha)
{
    //Get the layer palette.
    QPalette pal=m_shadowLayer->palette();
    pal.setColor(QPalette::Window,
                 QColor(0, 0, 0, //Black color
                        alpha * MaximumShadowDepth//Calculate the alpha
                        ));
    //Update the palette.
    m_shadowLayer->setPalette(pal);
}

inline void KNMailFolderViewer::startAnimeViewer(int endFrame)
{
    //Check viewer animation running state.
    if(m_viewerAnime->state()==QTimeLine::Running &&
            endFrame==m_viewerAnime->endFrame())
    {
        //Ignore the same request.
        return;
    }
    //Check the viewer pos.
    if(ViewerLeftSpacing == endFrame)
    {
        //Disconnect the hide connection.
        disconnect(m_viewerAnime, &QTimeLine::finished, 0, 0);
        //Update the is shown flag.
        m_isAnimeShown=true;
        //Show the widget.
        m_viewer->show();
        m_shadowLayer->show();
        //Reset the viewer position.
        m_viewer->move(width(), 0);
        updateViewerSize();
    }
    else
    {
        //When the viewer animation finished, hide the viewer anime.
        connect(m_viewerAnime, &QTimeLine::finished,
                m_viewer, &KNMailViewerBase::hide);
        connect(m_viewerAnime, &QTimeLine::finished,
                m_shadowLayer, &QWidget::hide);
        //Update the is shown flag.
        m_isAnimeShown=false;
    }
    //Stop the time line.
    m_viewerAnime->stop();
    //Update the frame.
    m_viewerAnime->setFrameRange(m_viewer->x(), endFrame);
    //Start the viewer.
    m_viewerAnime->start();
}
