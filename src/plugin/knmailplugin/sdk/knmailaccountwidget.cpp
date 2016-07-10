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
#include <QPainter>
#include <QTimeLine>
#include <QMouseEvent>

#include "knmailaccount.h"
#include "knmailglobal.h"
#include "knmailaccountwidget.h"

#include <QDebug>

#define IconSize 16
#define IconSpacing 4
#define RoundedRadius 3
#define ButtonSize 16
#define TextStart 24
#define TextHeight 12

KNMailAccountWidget::KNMailAccountWidget(KNMailAccount *account,
                                         QWidget *parent) :
    QWidget(parent),
    m_pressedPoint(QPoint()),
    m_expandAnime(new QTimeLine(200, this)),
    m_account(account),
    m_expanded(false),
    m_drawContent(false)
{
    //Set property.
    setFixedHeight(ItemHeight + ItemMargin);
    //Update the font size.
    QFont labelFont=font();
    labelFont.setPixelSize(TextHeight);
    setFont(labelFont);
    //Configure the time line.
    m_expandAnime->setUpdateInterval(33);
    m_expandAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_expandAnime, &QTimeLine::frameChanged,
            this, &KNMailAccountWidget::onActionResizePanel);
}

int KNMailAccountWidget::expandedHeight()
{
    return (m_account->folderCount() + 1) * ItemHeight + ItemMargin;
}

void KNMailAccountWidget::mousePressEvent(QMouseEvent *event)
{
    //Do original event.
    QWidget::mousePressEvent(event);
    //Save the click point.
    m_pressedPoint=event->pos();
}

void KNMailAccountWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //Do original event.
    QWidget::mouseReleaseEvent(event);
    //Check the position.
    if(rect().contains(m_pressedPoint))
    {
        //Check the pressed point.
        //Check the expanded state.
        if(m_expanded)
        {
            //!FIXME: add codes here.
            ;
        }
        else
        {
            //When the panel is fold, expand it.
            expandPanel();
        }
        //Reset the pressed point.
        m_pressedPoint=QPoint();
    }
}

void KNMailAccountWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Check the account pointer.
    if(!m_account)
    {
        //Ignore the painting when there's no account pointer.
        return;
    }
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Update the pen settings.
    painter.setPen(Qt::NoPen);
    //Get the palette.
    const QPalette &pal=palette();
    //Draw the base rounded rect.
    painter.setBrush(pal.color(QPalette::Base));
    painter.drawRoundedRect(QRect(0, 0, width(), height() - ItemSpacing),
                            RoundedRadius,
                            RoundedRadius);
    //Draw the top button.
    painter.setBrush(pal.color(QPalette::Window));
    painter.drawRoundedRect(QRect(0, 0, width(), ItemHeight),
                            RoundedRadius,
                            RoundedRadius);
    //Draw the icon.
    //!FIXME: should be from account.
    painter.drawPixmap(IconSpacing, IconSpacing,
                       knMailGlobal->providerIcon("netease"));
    //Draw the text.
    painter.setPen(pal.color(QPalette::Text));
    painter.drawText(TextStart, IconSpacing + TextHeight,
                     fontMetrics().elidedText(
                         m_account->displayString(),
                         Qt::ElideRight,
                         width()-TextStart-(ButtonSize+IconSpacing)*3));
}

void KNMailAccountWidget::onActionResizePanel(int currentHeight)
{
    //Resize the widget.
    setFixedHeight(currentHeight);
}

void KNMailAccountWidget::setExpand(bool expand)
{
    //Save the expand state.
    m_expanded=expand;
    //Check the expand state.
    if(m_expanded)
    {
        //Get the target height.
        int targetHeight=expandedHeight();
        //Switch to expand state.
        setFixedHeight(targetHeight);
        //Emit the signal.
        emit panelExpanded(targetHeight);
        //Mission complete.
        return;
    }
    //Switch to fold state.
    //Resize the widget.
    setFixedHeight(ItemHeight + ItemMargin);
    //Emit the signal.
    emit panelFolded();
}

void KNMailAccountWidget::foldPanel()
{
    //Save the expanded state.
    m_expanded=false;
    //Start the animation to the fold state.
    startHeightAnime(ItemHeight + ItemMargin);
    //Emit the signal.
    emit panelFolded();
}

void KNMailAccountWidget::expandPanel()
{
    //Save the expanded state.
    m_expanded=true;
    //Get the expanded height.
    int targetHeight=expandedHeight();
    //Start the animation to the expand state.
    startHeightAnime(targetHeight);
    //Emit the signal.
    emit panelExpanded(targetHeight);
}

inline void KNMailAccountWidget::startHeightAnime(int targetHeight)
{
    //Stop the time line,
    m_expandAnime->stop();
    //Update frame range.
    m_expandAnime->setFrameRange(height(), targetHeight);
    //Start the time line.
    m_expandAnime->start();
}
