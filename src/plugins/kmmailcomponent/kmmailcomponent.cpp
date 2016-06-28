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
#include <QScrollArea>
#include <QScrollBar>

#include "knthememanager.h"
#include "mime/kmmimepart.h"
#include "mime/kmmimemailparser.h"
#include "mime/kmmailparseutil.h"
#include "sao/knsaostyle.h"

#include "kmmailcomponenttitlebar.h"
#include "sdk/kmmailcomponentcontentbase.h"

#ifdef BACKEND_WEBENGINE
#include "plugins/kmmailcomponentwebengine/kmmailcomponentwebengine.h"
#endif
#ifdef BACKEND_WEBKIT
#include "plugins/kmmailcomponentwebkit/kmmailcomponentwebkit.h"
#endif

#include "kmmailcomponent.h"

#include <QDebug>

#define ScrollBarWidth 10
#define MaxOpacity 0x30

KMMailComponent::KMMailComponent(QWidget *parent) :
    KMMailComponentBase(parent),
    m_titleBar(new KMMailComponentTitleBar(this)),
    m_content(nullptr),
    m_mimePart(nullptr)
{
    setObjectName("MailComponent");
    //Set properties.
    setAutoFillBackground(true);
    //Register the widget.
    knTheme->registerWidget(this);
    //Initial the content widget.
#ifdef BACKEND_WEBENGINE
    m_content=new KMMailComponentWebEngine(this);
#endif
#ifdef BACKEND_WEBKIT
    m_content=new KMMailComponentWebKit(this);
#endif
    //Check the content data.
    if(!m_content)
    {
        //No content, we cannot continue to load data.
        return;
    }
    //Configure the title bar.
    connect(m_titleBar, &KMMailComponentTitleBar::titleSizeUpdate,
            this, &KMMailComponent::updateGeometries);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    //Set layout to container.
    setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_titleBar);
    //Add component widget to layout.
    mainLayout->addWidget(m_content, 1);
}

KMMailComponent::~KMMailComponent()
{
    //Recover the mime part memory.
    recoveryMimePart();
}

void KMMailComponent::reset()
{
    //Recover the mime part memory.
    recoveryMimePart();
    //Reset the title bar.
    m_titleBar->reset();
    m_content->reset();
}

void KMMailComponent::loadMail(const QString &filePath)
{
    //Reset the component first.
    reset();
    //Load the mail file.
    m_mimePart=KMMimeMailParser::parseContent(filePath, nullptr);
    //Get the title data.
    m_titleBar->setTitle(KMMailParseUtil::parseEncoding(
                             m_mimePart->mimeProperty("subject")));
    //Build the from address string list.
    QString fromAddress=
            KMMailParseUtil::parseEncoding(m_mimePart->mimeProperty("from"));
    {
        //Parse the data.
        int addressStart=fromAddress.indexOf('<');
        //Check result first.
        if(addressStart!=-1)
        {
            //Insert a spacing between the name and address.
            fromAddress=fromAddress.left(addressStart).simplified() + " "
                    + fromAddress.mid(addressStart).simplified();
        }
    }
    //Construct the from list.
    QStringList fromList;
    fromList.append(fromAddress);
    m_titleBar->setSenderList(fromList);
    //Construct the to list.
    QStringList toList;
    {
        //Get the target address.
        QString toAddress=
                KMMailParseUtil::parseEncoding(m_mimePart->mimeProperty("to"));
        //Split the to address.
        toList=toAddress.split(", ", QString::SkipEmptyParts);
        //Check all the parts of the to list.
        for(int i=0; i<toList.size(); ++i)
        {
            //Get the current item.
            QString currentAddress=toList.at(i);
            //Parse the data.
            int addressStart=currentAddress.indexOf('<');
            //Check result first.
            if(addressStart!=-1)
            {
                //Insert a spacing between the name and address.
                currentAddress=currentAddress.left(addressStart).simplified() +
                        " " + currentAddress.mid(addressStart).simplified();
                //Replace the current address.
                toList.replace(i, currentAddress);
            }
        }
    }
    m_titleBar->setReceiverList(toList);
    //Get the receive date from the data.
    m_titleBar->setReceiveDate(
                KMMimeMailParser::getDate(m_mimePart->mimeProperty("date")));
}

void KMMailComponent::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KMMailComponentBase::resizeEvent(event);
    //Update the geometry.
    updateGeometries();
}

void KMMailComponent::updateGeometries()
{
    //Update the title bar width.
    m_titleBar->resize(width(), m_titleBar->height());
    //Check the content pointer first.
    if(m_content)
    {
        //Update the content size.
        m_content->setGeometry(0,
                               m_titleBar->height(),
                               width(),
                               height()-m_titleBar->height());
    }
}

inline void KMMailComponent::recoveryMimePart()
{
    //Check the mime part data.
    if(m_mimePart)
    {
        //Recover the memory.
        delete m_mimePart;
        //Reset the pointer.
        m_mimePart=nullptr;
    }
}
