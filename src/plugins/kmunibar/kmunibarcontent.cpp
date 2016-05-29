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

#include "kmunibaraccountlist.h"
#include "mailaccount/kmmailaccount.h"
#include "mailaccount/kmmailaccountmanager.h"

#include "kmunibarcontent.h"

#include <QDebug>

KMUnibarContent::KMUnibarContent(QWidget *parent) :
    QWidget(parent),
    m_mainLayout(new QBoxLayout(QBoxLayout::TopToBottom))
{
    //Initial the content layout.
    QBoxLayout *contentLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(contentLayout);
    //Add main layout to content layout.
    contentLayout->addLayout(m_mainLayout);
    contentLayout->addStretch();
    //Configure layout.
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    //Link with the account manager.
    connect(kmMailAccount, &KMMailAccountManager::accountAppend,
            this, &KMUnibarContent::onActionAccountAdd);
}

void KMUnibarContent::addAccountList(KMUnibarAccountList *accountList)
{
    //Add account list to main layout.
    m_mainLayout->addWidget(accountList);
    //Link the account list.
    connect(accountList, &KMUnibarAccountList::sizeChanged,
            this, &KMUnibarContent::onActionChangeSize);
    connect(accountList, &KMUnibarAccountList::currentModelChanged,
            this, &KMUnibarContent::onActionChangeModel);
}

void KMUnibarContent::onActionChangeSize(int heightDelta)
{
    resize(width(), height()+heightDelta);
}

void KMUnibarContent::onActionChangeModel(int modelIndex)
{
    //Get the sender pointer.
    KMUnibarAccountList *accountList=
            static_cast<KMUnibarAccountList *>(sender());
    //Re-emit signal.
    emit switchModel(accountList->account(), modelIndex);
    //Update the title bar text.
    emit requireUpdateTitle(accountList->folderText(modelIndex));
}

void KMUnibarContent::onActionAccountAdd(KMMailAccount *account)
{
    //Insert generate the account list.
    KMUnibarAccountList *accountList=new KMUnibarAccountList(this);
    //Set the account backend to account list.
    accountList->setAccount(account);
    //Add account list to widget.
    addAccountList(accountList);
}
