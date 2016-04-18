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
#include <QPropertyAnimation>

#include "knsideshadowwidget.h"
#include "knthememanager.h"
#include "kmtitlebarcombo.h"
#include "kmtitlebarbase.h"

#include "kmunibar.h"

#define ShadowHeight 15

KMUnibar::KMUnibar(QWidget *parent) :
    KMUnibarBase(parent),
    m_titleBar(nullptr),
    m_shadowCombo(new KMTitleBarCombo(this)),
    m_titleBarCombo(nullptr),
    m_sizeAnimation(new QPropertyAnimation(this, "size", this)),
    m_topShadow(new KNSideShadowWidget(KNSideShadowWidget::TopShadow,
                                       this)),
    m_rightShadow(new KNSideShadowWidget(KNSideShadowWidget::RightShadow,
                                         this))
{
    setObjectName("UniBar");
    //Set properties.
    setAutoFillBackground(true);
    //Configure the combo button.
    m_shadowCombo->setEnabled(false);
    //Configure the animation.
    m_sizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_sizeAnimation->setDuration(200);
    //Configure the side shadow.
    m_topShadow->move(0, m_shadowCombo->height());
    m_topShadow->setFixedHeight(ShadowHeight);
    m_rightShadow->hide();

    //Register the widget.
    knTheme->registerWidget(this);
}

void KMUnibar::setTitleBar(KMTitleBarBase *titleBar)
{
    //Check the title bar first.
    if(!titleBar)
    {
        //Ignore the null pointer.
        return;
    }
    //Save the title bar pointer.
    m_titleBar=titleBar;
    //Save the title bar combo widget pointer.
    m_titleBarCombo=m_titleBar->titleCombo();
}

void KMUnibar::showUnibar(QSize mainWindowSize)
{
    //Sync the data from the titlbar combo.
    m_shadowCombo->setUserAvatar(m_titleBarCombo->userAvatar());
    m_shadowCombo->setText(m_titleBarCombo->text());
    //Get the title bar combo widget size.
    QSize titleBarComboSize=m_titleBarCombo->size();
    //Resize the unibar to be title combo.
    m_shadowCombo->resize(titleBarComboSize);
    //Change the current size.
    resize(titleBarComboSize);
    //Update and launch the animation.
    m_sizeAnimation->setStartValue(titleBarComboSize);
    m_sizeAnimation->setEndValue(QSize(m_titleBar->width(),
                                       mainWindowSize.height()));
    //Start animation.
    m_sizeAnimation->start();
}

void KMUnibar::hideUnibar()
{
    //Update and launch the animation.
    m_sizeAnimation->setStartValue(size());
    m_sizeAnimation->setEndValue(m_titleBarCombo->size());
    //Link the size animation signal.
    connect(m_sizeAnimation, &QPropertyAnimation::finished,
            this, &KMUnibar::onActionHideUnibar);
    //Start animation.
    m_sizeAnimation->start();
}

void KMUnibar::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    KMUnibarBase::resizeEvent(event);
    //Resize the combo.
    m_shadowCombo->resize(width() + 1, m_shadowCombo->height());
    //Resize the top shadow.
    m_topShadow->resize(width(), m_topShadow->height());
}

void KMUnibar::onActionHideUnibar()
{
    //Disconnect the finish signal.
    disconnect(m_sizeAnimation, &QPropertyAnimation::finished, 0, 0);
    //Hide current widget.
    hide();
}
