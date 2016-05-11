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
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QTimeLine>

#include "kmunibarlabelbutton.h"
#include "kmunibarbutton.h"
#include "knlocalemanager.h"

#include "kmunibaraccountlist.h"

#include <QDebug>

#define LineHeight 38

KMUnibarAccountList::KMUnibarAccountList(QWidget *parent) :
    QWidget(parent),
    m_accountLabel(QString()),
    m_foldedButton(new KMUnibarLabelButton(this)),
    m_animeTimeLine(new QTimeLine(200, this)),
    m_currentFolder(-1),
    m_expand(false)
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    setFixedHeight(LineHeight);
    //Initial the system folder buttons.
    for(int i=0; i<MailSystemFoldersCount; ++i)
    {
        //Initial the button.
        m_systemFolder[i]=new KMUnibarButton(this);
        //Configure the button.
        m_systemFolder[i]->hide();
        m_systemFolder[i]->setEnabled(false);
        m_systemFolder[i]->setCheckable(true);
        //Add system folder to folder list.
        addToFolderList(m_systemFolder[i]);
    }
    //Configure fold button.
    m_foldedButton->setFixedSize(LineHeight, LineHeight);
    m_foldedButton->setImage(QPixmap("://image/public/right_arrow.png"));
    connect(m_foldedButton, &KMUnibarLabelButton::clicked,
            this, &KMUnibarAccountList::onActionChangeExpand);
    //Configure time line.
    m_animeTimeLine->setEasingCurve(QEasingCurve::OutCubic);
    m_animeTimeLine->setUpdateInterval(33);
    connect(m_animeTimeLine, &QTimeLine::frameChanged,
            this, &KMUnibarAccountList::onActionExpandWidget);

    //Initial retranslate.
    knI18n->link(this, &KMUnibarAccountList::retranslate);
    retranslate();
}

QString KMUnibarAccountList::accountLabel() const
{
    return m_accountLabel;
}

void KMUnibarAccountList::setAccountLabel(const QString &accountLabel)
{
    //Save the label.
    m_accountLabel = accountLabel;
    //Update the widget.
    update();
}

void KMUnibarAccountList::paintEvent(QPaintEvent *event)
{
    //Draw original widget.
    QWidget::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the first line.
    painter.setPen(Qt::NoPen);
    painter.setBrush(palette().color(QPalette::Window));
    painter.drawRect(0, 0, width(), LineHeight);
    //Draw the first line text.
    int firstLineWidth=width()-LineHeight;
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawText(LineHeight, 0, firstLineWidth, LineHeight,
                     Qt::AlignLeft | Qt::AlignVCenter,
                     m_accountLabel);
}

void KMUnibarAccountList::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Resize the folder buttons.
    for(int i=0; i<m_folderList.size(); ++i)
    {
        //Resize the widget.
        m_folderList.at(i)->resize(width(), LineHeight);
    }
}

void KMUnibarAccountList::retranslate()
{
    //Update system folder label.
    m_systemFolder[FolderInbox]->setText(tr("Inbox"));
    m_systemFolder[FolderSentItems]->setText(tr("Sent Items"));
    m_systemFolder[FolderDrafts]->setText(tr("Drafts"));
    m_systemFolder[FolderTrash]->setText(tr("Trash"));
}

void KMUnibarAccountList::onActionExpandWidget(int widgetHeight)
{
    //Calculate the percentage.
    qreal percentage=(qreal)(widgetHeight - LineHeight) /
                     (qreal)(MailSystemFoldersCount * LineHeight);
    //Update the rotation.
    m_foldedButton->setRotate(percentage * (qreal)90.0);
    //Update folder button.
    for(int i=0; i<m_folderList.size(); ++i)
    {
        //Get the folder widget.
        KMUnibarButton *folder=m_folderList.at(i);
        //Hide the folder widget.
        folder->setOpacity(percentage);
        //Move the folder widget.
        folder->move(0, percentage * LineHeight * (i+1));
    }
    //Update the height.
    setFixedHeight(widgetHeight);
}

void KMUnibarAccountList::onActionChangeExpand()
{
    //Check the running state.
    if(m_animeTimeLine->state()==QTimeLine::Running)
    {
        //Ignore the request when running the animation.
        return;
    }
    //Check the expand state.
    if(m_expand)
    {
        //Disable all folder widget.
        for(int i=0; i<m_folderList.size(); ++i)
        {
            //Disabled all the widget.
            m_folderList.at(i)->setEnabled(false);
        }
        //We will fold to one line.
        connect(m_animeTimeLine, &QTimeLine::finished,
                this, &KMUnibarAccountList::onActionHideFinished);
        //Start the anime.
        startAnime(LineHeight);
        //Change the flag.
        m_expand=false;
        //Mission complete.
        return;
    }
    //Launch the animation.
    //Show all the folder.
    for(int i=0; i<m_folderList.size(); ++i)
    {
        //Show the folder widget.
        m_folderList.at(i)->show();
    }
    //We will fold to one line.
    connect(m_animeTimeLine, &QTimeLine::finished,
            this, &KMUnibarAccountList::onActionShowFinished);
    //Emit height changed signal.
    emit sizeChanged(MailSystemFoldersCount*LineHeight);
    //Start the anime.
    startAnime(LineHeight*(MailSystemFoldersCount+1));
    //Change the flag.
    m_expand=true;
    //Mission complete.
    return;
}

void KMUnibarAccountList::onActionShowFinished()
{
    //Disconnect time line.
    disconnect(m_animeTimeLine, &QTimeLine::finished, 0, 0);
    //Enabled all the folder.
    for(int i=0; i<m_folderList.size(); ++i)
    {
        //Enabled the folder widget.
        m_folderList.at(i)->setEnabled(true);
    }
}

void KMUnibarAccountList::onActionHideFinished()
{
    //Disconnect time line.
    disconnect(m_animeTimeLine, &QTimeLine::finished, 0, 0);
    //Hide all the folder.
    for(int i=0; i<m_folderList.size(); ++i)
    {
        //Hide the folder widget.
        m_folderList.at(i)->hide();
    }
    //Emit height changed signal.
    emit sizeChanged(-MailSystemFoldersCount*LineHeight);
}

void KMUnibarAccountList::onActionButtonClicked()
{
    //Get the sender button.
    KMUnibarButton *folderButton=static_cast<KMUnibarButton *>(sender());
    //Get the button index.
    int buttonIndex=m_folderList.indexOf(folderButton);
    //Check the current index.
    if(m_currentFolder>-1)
    {
        //Reset the current folder state.
        m_folderList.at(m_currentFolder)->setChecked(false);
    }
    //Save the current index.
    m_currentFolder=buttonIndex;
    //Emit the model changed signal.
    emit currentModelChanged(m_currentFolder);
}

inline void KMUnibarAccountList::addToFolderList(KMUnibarButton *button)
{
    //Add the widget to list.
    m_folderList.append(button);
    //Check the current state.
    button->move(0, m_expand?(m_folderList.size()*LineHeight):0);
    //Link the button to current widget.
    connect(button, SIGNAL(clicked(bool)), this, SLOT(onActionButtonClicked()));
}

inline void KMUnibarAccountList::startAnime(int endFrame)
{
    //Stop animation.
    m_animeTimeLine->stop();
    //Update range.
    m_animeTimeLine->setFrameRange(height(), endFrame);
    //Start animation.
    m_animeTimeLine->start();
}