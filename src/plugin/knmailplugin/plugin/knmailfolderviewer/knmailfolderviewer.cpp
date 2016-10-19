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

#include "knmailmodelupdater.h"
#include "knmailaccount.h"
#include "knmailmodel.h"
#include "knmailviewerbase.h"
#include "knmailfolderviewertitle.h"
#include "knmailfolderproxymodel.h"
#include "knmailfoldertreeview.h"

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
    m_proxyModel(new KNMailFolderProxyModel(this)),
    m_viewerAnime(new QTimeLine(200, this)),
    m_isAnimeShown(true)
{
    setObjectName("MailFolderViewer");
    //Set properties.
    setAutoFillBackground(true);
    //Register to the theme manager.
    knTheme->registerWidget(this);
    //Link with the updater.
    connect(this, &KNMailFolderViewer::requireUpdateFolder,
            knMailModelUpdater, &KNMailModelUpdater::startUpdateFolder,
            Qt::QueuedConnection);
    //Update the proxy model.
    m_proxyModel->setPageSize(5);
    connect(m_proxyModel, &KNMailFolderProxyModel::requireUpdate,
            this, &KNMailFolderViewer::onUpdateItems, Qt::QueuedConnection);
    //Configure the folder view.
    m_folderView->setObjectName("");
    m_folderView->setModel(m_proxyModel);
    //Link the selection model selected signal to show the viewer.
    connect(m_folderView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this, &KNMailFolderViewer::onSelectionChange);
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
    //Check the previous model.
    if(m_proxyModel->sourceModel())
    {
        //Disconnect with its previous signal.
        disconnect(static_cast<KNMailModel *>(m_proxyModel->sourceModel()),
                   0, 0, 0);
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
    m_proxyModel->setSourceModel(folderModel);
    //Update the model.
    ;
    //Link the folder model.
    connect(folderModel, &KNMailModel::modelUpdated,
            this, &KNMailFolderViewer::onModelUpdate);
    //Reset the page index.
    m_proxyModel->setPageIndex(0);
    //Update the folder.
    onUpdateItems(0, m_proxyModel->pageSize());
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

void KNMailFolderViewer::onModelUpdate()
{
    //Update the current page, set the current index to the model.
    m_proxyModel->setPageIndex(m_proxyModel->pageIndex());
}

void KNMailFolderViewer::onUpdateItems(int startPosition, int endPosition)
{
    //Get current model.
    KNMailModel *currentModel=(KNMailModel *)(m_proxyModel->sourceModel());
    //Update the data via the updater.
    emit requireUpdateFolder(currentModel->managedAccount(),
                             currentModel,
                             startPosition,
                             endPosition);
}

void KNMailFolderViewer::onSelectionChange(const QModelIndex &current)
{
    Q_UNUSED(current)
    //Check the current index validation.
    if(!current.data(MailCachedRole).toBool())
    {
        //Ignore the mail which is not cached.
        return;
    }
    //Get the model parent, it should be the account.
    KNMailModel *currentModel=(KNMailModel *)(m_proxyModel->sourceModel());
    KNMailAccount *mailAccount=currentModel->managedAccount();
    m_viewer->loadMail(mailAccount->accountDirectoryPath() + "/" +
                       currentModel->folderName() + "/" +
                       current.data(MailPathRole).toString()+".eml");
    //Check viewer state.
    if(!m_viewer->isVisible())
    {
        //Show the viewer.
        m_viewer->show();
        m_shadowLayer->show();
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
                        alpha * MaximumShadowDepth //Calculate the alpha
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
