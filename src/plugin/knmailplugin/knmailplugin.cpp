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
#include <QStackedLayout>

//Dependencies.
#include "knthememanager.h"

//Mail plugin sdks.
#include "knmailglobal.h"
#include "knmailaccountlist.h"
#include "knmailemptyhint.h"

#include "knmailplugin.h"

//Debug
#include "knmailaccount.h"
#include "knmailaccountmanager.h"

KNMailPlugin::KNMailPlugin(QWidget *parent) :
    KNMailPluginBase(parent),
    m_leftBarContainer(nullptr),
    m_mainLayout(new QStackedLayout(this))
{
    //Initial the basic infrastructure.
    initialInfrastructure();
}

QWidget *KNMailPlugin::accountPanel()
{
    return m_leftBarContainer;
}

void KNMailPlugin::loadPlugins()
{
    //Debug
    KNMailAccount *account=new KNMailAccount(this);
    account->setDisplayName("Mimami Kotori");
    account->setUsername("kotori@ll-anime.com");
    account->setProvider("netease");
    knMailAccountManager->appendAccount(account);

    account=new KNMailAccount(this);
    account->setDisplayName("Sonoda Umi");
    account->setUsername("umi@ll-anime.com");
    account->setProvider("google");
    knMailAccountManager->appendAccount(account);

    account=new KNMailAccount(this);
    account->setDisplayName("Kosaka Honoka");
    account->setUsername("honoka@ll-anime.com");
    account->setProvider("netease");
    knMailAccountManager->appendAccount(account);
}

inline void KNMailPlugin::initialInfrastructure()
{
    //Initial the mail plugin global.
    KNMailGlobal::initial(this);

    //Load the left bar container.
    m_leftBarContainer=new KNMailAccountList(this);
    //Configure the layout.
    m_mainLayout->setContentsMargins(0,0,0,0);
    setLayout(m_mainLayout);
    //Initial the empty hint widget.
    m_mainLayout->addWidget(new KNMailEmptyHint(this));
}
