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
#include "sao/knsaostyle.h"

#include "kmmailcomponenttitlebar.h"
#include "sdk/kmmailcomponentcontentbase.h"

#ifdef BACKEND_WEBENGINE
#include "plugins/kmmailcomponentwebengine/kmmailcomponentwebengine.h"
#endif

#include "kmmailcomponent.h"

#include <QDebug>

#define ScrollBarWidth 10
#define MaxOpacity 0x30

KMMailComponent::KMMailComponent(QWidget *parent) :
    KMMailComponentBase(parent),
    m_mouseAnime(new QTimeLine(200, this)),
    m_mailContentArea(new QScrollArea(this)),
    m_scrollBar(new QScrollBar(m_mailContentArea)),
    m_container(new QWidget(this)),
    m_titleBar(new KMMailComponentTitleBar(this)),
    m_content(nullptr)
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
    //Set the container to the content area.
    m_mailContentArea->setWidget(m_container);
    m_mailContentArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_mailContentArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_mailContentArea->setFrameStyle(QFrame::NoFrame);
    //Configure the scroll bar.
    m_scrollBar->hide();
    m_scrollBar->setStyle(KNSaoStyle::instance());
    m_scrollBar->setSingleStep(27);
    m_scrollBar->setPageStep(27);
    m_scrollBar->setObjectName("MailComponentScrollBar");
    connect(knTheme, &KNThemeManager::themeChange,
            [=]
            {
                //Get the new palette from theme manager, and set it.
                m_scrollBar->setPalette(knTheme->getPalette(
                                            m_scrollBar->objectName()));
                //Update the palette.
                onActionMouseInOut(0);
            });
    //Update the validation.
    m_scrollBar->setPalette(knTheme->getPalette(m_scrollBar->objectName()));
    onActionMouseInOut(0);

    //Update the single step.
    m_mailContentArea->verticalScrollBar()->setSingleStep(27);

    connect(m_mailContentArea->verticalScrollBar(), &QScrollBar::rangeChanged,
            this, &KMMailComponent::onActionRangeChange);
    connect(m_mailContentArea->verticalScrollBar(), &QScrollBar::valueChanged,
            [=](int value)
            {
                //Block the signal.
                m_scrollBar->blockSignals(true);
                //Reset the value.
                m_scrollBar->setValue(value);
                //Release the block
                m_scrollBar->blockSignals(false);
            });
    connect(m_scrollBar, &QScrollBar::valueChanged,
            m_mailContentArea->verticalScrollBar(), &QScrollBar::setValue);
    //Configure title bar.
    connect(m_titleBar, &KMMailComponentTitleBar::titleSizeUpdate,
            [=]
            {
                //Update the geometries.
                updateGeometries();
            });

    //Configure the time line.
    m_mouseAnime->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseAnime->setUpdateInterval(10);
    //Link the time line.
    connect(m_mouseAnime, &QTimeLine::frameChanged,
            this, &KMMailComponent::onActionMouseInOut);

    //Initial the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    //Set layout to container.
    m_container->setLayout(mainLayout);
    //Add widget to layout.
    mainLayout->addWidget(m_titleBar);
    //Add component widget to layout.
    mainLayout->addWidget(m_content, 1);

    //Debug.
    m_titleBar->setTitle("TechLauncher: First peer assessment is due at 17:00 today");
    m_titleBar->setReceiveDate(QDate(2016,3,1));
    //Debug
    QStringList receiverList;
    receiverList << "ayase.eri@ll-anime.com" << "tojo.nozomi@ll-anime.com";
    m_titleBar->setReceiverList(receiverList);
    receiverList << "yazawa.nico@ll-anime.com"
                 << "minami.kotori@ll-anime.com"
                 << "sonoda.umi@ll-anime.com"
                 << "nishikino.maki@ll-anime.com"
                 << "koitsumi.hanayo@ll-anime.com"
                 << "hoshizora.rin@ll-anime.com"
                 << "kousaka.honoka@ll-anime.com"
                 << "nitta.emi@ll-anime.com"
                 << "mimorin.suzuko@ll-anime.com"
                 << "utchida.aya@ll-anime.com"
                 << "kubo.yurika@ll-anime.com"
                 << "itta.riho@ll-anime.com"
                 << "pile@ll-anime.com"
                 << "toukui.sora@ll-anime.com"
                 << "nanjo.yoshino@ll-anime.com"
                 << "kusuda.aina@ll-anime.com";
    m_titleBar->setReceiverList(receiverList);
    QStringList senderList;
    senderList << "tojo.nozomi@ll-anime.com";
    m_titleBar->setSenderList(senderList);
}

void KMMailComponent::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KMMailComponentBase::resizeEvent(event);
    //Update geometries.
    updateGeometries();
}

void KMMailComponent::enterEvent(QEvent *event)
{
    //Enter the widget.
    KMMailComponentBase::enterEvent(event);
    //Start mouse in anime.
    startAnime(MaxOpacity);
}

void KMMailComponent::leaveEvent(QEvent *event)
{
    //Enter the widget.
    KMMailComponentBase::leaveEvent(event);
    //Start mouse in anime.
    startAnime(0);
}

void KMMailComponent::onActionMouseInOut(int frame)
{
    //Update the scroll bar color.
    QPalette pal=m_scrollBar->palette();
    QColor color=pal.color(QPalette::Base);
    color.setAlpha(frame);
    pal.setColor(QPalette::Base, color);
    color=pal.color(QPalette::Button);
    color.setAlpha(frame<<1);
    pal.setColor(QPalette::Button, color);
    //Set the palette to scroll bar.
    m_scrollBar->setPalette(pal);
}

void KMMailComponent::onActionRangeChange(int min, int max)
{
    //Update the range first.
    m_scrollBar->setRange(min, max);
    //Check whether the scroll bar is still valid.
    m_scrollBar->setVisible(min!=max);
}

void KMMailComponent::updateGeometries()
{
    //Update the scroll bar position.
    m_scrollBar->setGeometry(width()-ScrollBarWidth,
                             0,
                             ScrollBarWidth,
                             height());
    //Resize the content area.
    m_mailContentArea->resize(size());
    //Update title bar.
    m_titleBar->updateHeight(width());
    //Update the content size.
    //Check the title bar states.
    if(m_titleBar->isExpand())
    {
        //If the title bar is expand, then the content will make it to be the
        //whole size.
        m_content->setFixedHeight(height());
    }
    else
    {
        //Resize the content widget.
        m_content->setFixedHeight(height()-m_titleBar->height());
    }
    //Recalculate the container size.
    m_container->resize(width(), m_container->sizeHint().height());
}

inline void KMMailComponent::startAnime(int endFrame)
{
    //Stop the mouse animations.
    m_mouseAnime->stop();
    //Set the parameter of the time line.
    m_mouseAnime->setFrameRange(
                m_scrollBar->palette().color(QPalette::Base).alpha(),
                endFrame);
    //Start the time line.
    m_mouseAnime->start();
}
