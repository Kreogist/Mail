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
#include "knsideshadowwidget.h"
#include "kmmaillistview.h"
#include "kmmailutil.h"
#include "kmmaillistmodel.h"
#include "mailaccount/kmmailaccount.h"

#include "kmleftbar.h"

#define ShadowHeight 10

using namespace MailUtil;

KMLeftBar::KMLeftBar(QWidget *parent) :
    KMLeftBarBase(parent),
    m_mailList(new KMMailListView(this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadowWidget::TopShadow, this))
{
    //Set fixed shadow height.
    m_topShadow->setFixedHeight(ShadowHeight);
    //Configure the mail list view.
    m_mailList->updateObjectName("LeftBarMailList");
}

void KMLeftBar::setMailListModel(KMMailListModel *model)
{
    //Set the model to list view.
    m_mailList->setModel(model);
}

void KMLeftBar::switchModel(KMMailAccount *account, int modelIndex)
{
    //Set the current mail list model to the target model.
    setMailListModel(
                modelIndex>MailSystemFoldersCount?
                    account->customFolder(modelIndex-MailSystemFoldersCount):
                    account->systemFolder(modelIndex));
}

void KMLeftBar::resizeEvent(QResizeEvent *event)
{
    //Do the resize Event.
    KMLeftBarBase::resizeEvent(event);
    //Resize the shadow.
    m_topShadow->resize(width(), ShadowHeight);
    //Resize the mail list.
    m_mailList->resize(size());
}
