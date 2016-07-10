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
#include "knmailaccountbutton.h"
#include "knmailaccountwidget.h"
#include "knmailglobal.h"

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
    m_expandAnime(new QTimeLine(200, this)),
    m_account(account),
    m_selectedIndex(-1),
    m_expanded(false),
    m_drawContent(false),
    m_isPressed(false)
{
    //Set property.
    setFixedHeight(ItemHeight + ItemMargin);
    //Initial the button.
    for(int i=0; i<MailAccountButtonCount; ++i)
    {
        //Initial the button.
        m_button[i]=new KNMailAccountButton(this);
        //Resize the button.
        m_button[i]->setFixedSize(ButtonSize, ButtonSize);
    }
    //Update the button image.
    m_button[ButtonSync]->setIcon(
                QIcon(":/plugin/mail/account/account_update.png"));
    m_button[ButtonOption]->setIcon(
                QIcon(":/plugin/mail/account/account_config.png"));
    m_button[ButtonExpand]->setIcon(
                QIcon(":/plugin/mail/account/account_expand.png"));
    //Link the button.
    connect(m_button[ButtonExpand], &KNMailAccountButton::clicked,
            this, &KNMailAccountWidget::expandPanel);
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
    m_isPressed=true;
}

void KNMailAccountWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //Do original event.
    QWidget::mouseReleaseEvent(event);
    //Check the position.
    if(m_isPressed && rect().contains(event->pos()))
    {
        //Check whether the panel is expanded.
        if(m_expanded)
        {
            //Check whether the widget is displaying the widget.
            if(m_drawContent)
            {
                //Check the click position, calculate the model index.
                int clickedIndex=(event->pos().y()/ItemHeight)-1;
                //Check the index is valid or not, then check the click position
                //with the current index.
                if(clickedIndex>-1 && clickedIndex!=m_selectedIndex)
                {
                    //Update the selected index.
                    m_selectedIndex=clickedIndex;
                    //Update the widget.
                    update();
                    //Emit the model switch signal.
                    //!FIXME: Add codes here!
                }
            }
        }
        else
        {
            //When the panel is fold, expand it.
            expandPanel();
        }
        //Reset the pressed point.
        m_isPressed=false;
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
    painter.drawRoundedRect(QRect(0, 0, width(), height() - ItemMargin),
                            RoundedRadius,
                            RoundedRadius);
    //Draw the content when it is on.
    if(m_drawContent)
    {
        //Get the text pen and line pen.
        QPen textPen=palette().color(QPalette::Text),
                linePen=palette().color(QPalette::Window);
        //Draw all the folder.
        for(int i=0, folderCount=m_account->folderCount(),
            currentY=ItemHeight, textWidth=width()-(ItemSpacing<<2);
            i<folderCount;
            ++i)
        {
            //Check the folder index.
            if(m_selectedIndex==i)
            {
                //Remove the pen.
                painter.setPen(Qt::NoPen);
                painter.setBrush(palette().color(QPalette::Highlight));
                painter.setOpacity(0.5);
                //Draw the selector.
                painter.drawRect(QRect(0, currentY, width(), ItemHeight));
                painter.setOpacity(1.0);
            }
            //Update the pen.
            painter.setPen(textPen);
            //Draw the folder name.
            painter.drawText(QRect(ItemSpacing<<1, currentY,
                                   textWidth, ItemHeight),
                             Qt::AlignVCenter | Qt::AlignLeft,
                             fontMetrics().elidedText(m_account->folderName(i),
                                                      Qt::ElideRight,
                                                      textWidth));
            //Increase the current Y.
            currentY+=ItemHeight;
            //Draw the split line.
            if(i!=folderCount-1)
            {
                //Set the line pen.
                painter.setPen(linePen);
                //Draw the splitter line.
                painter.drawLine(QPoint(ItemSpacing, currentY),
                                 QPoint(width()-ItemSpacing, currentY));
            }
        }
        //Reset the pen and opacity.
        painter.setPen(Qt::NoPen);
    }
    //Draw the top button.
    painter.setBrush(pal.color(QPalette::Window));
    painter.drawRoundedRect(QRect(0, 0, width(), ItemHeight),
                            RoundedRadius,
                            RoundedRadius);
    //Draw the icon.
    painter.drawPixmap(IconSpacing, IconSpacing,
                       knMailGlobal->providerIcon(m_account->provider()));
    //Draw the text.
    painter.setPen(pal.color(QPalette::Text));
    painter.drawText(TextStart, IconSpacing + TextHeight,
                     fontMetrics().elidedText(
                         m_account->displayString(),
                         Qt::ElideRight,
                         width()-TextStart-(ButtonSize+IconSpacing)*3));
}

void KNMailAccountWidget::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QWidget::resizeEvent(event);
    //Calculate the initial button position.
    int buttonX=width()-MailAccountButtonCount*(ItemMargin+ButtonSize);
    //For all the buttons.
    for(int i=0; i<MailAccountButtonCount; ++i)
    {
        //Update the position of the button.
        m_button[i]->move(buttonX, IconSpacing);
        //Update the button X.
        buttonX+=(ItemMargin+ButtonSize);
    }
}

void KNMailAccountWidget::onActionResizePanel(int currentHeight)
{
    //Update the rotation.
    m_button[ButtonExpand]->setRotate(
                (qreal)(currentHeight-ItemHeight) /
                (qreal)(expandedHeight()-ItemHeight)*90.0);
    //Resize the widget.
    setFixedHeight(currentHeight);
}

inline void KNMailAccountWidget::updateExpandParameters()
{
    //Save the expanded state.
    m_expanded=true;
    //Update the selector data.
    m_selectedIndex=0;
}

inline void KNMailAccountWidget::updateFoldParameters()
{
    //Disable draw content.
    m_drawContent=false;
    //Save the expanded state.
    m_expanded=false;
    //Reset the selected models.
    m_selectedIndex=-1;
}

void KNMailAccountWidget::setExpand(bool expand)
{
    //Check the expand state.
    if(expand)
    {
        //Update expand parameters.
        updateExpandParameters();
        //Set the draw content to true.
        m_drawContent=true;
        //Update the button expand rotation.
        m_button[ButtonExpand]->setRotate(90);
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
    //Update fold parameters.
    updateFoldParameters();
    //Update the button expand rotation.
    m_button[ButtonExpand]->setRotate(0);
    //Resize the widget.
    setFixedHeight(ItemHeight + ItemMargin);
    //Emit the signal.
    emit panelFolded();
}

void KNMailAccountWidget::foldPanel()
{
    //Update fold parameters.
    updateFoldParameters();
    //Start the animation to the fold state.
    startHeightAnime(ItemHeight + ItemMargin);
    //Emit the signal.
    emit panelFolded();
}

void KNMailAccountWidget::expandPanel()
{
    //Check the expanded state.
    if(m_expanded)
    {
        //No need to expand again.
        return;
    }
    //Update the expand parameters.
    updateExpandParameters();
    //Get the expanded height.
    int targetHeight=expandedHeight();
    //Link to enable content.
    connect(m_expandAnime, &QTimeLine::finished,
            [=]
            {
                //Cut all the finish connection.
                disconnect(m_expandAnime, &QTimeLine::finished, 0, 0);
                //Enabled to draw the content.
                m_drawContent=true;
                //Update the widget.
                update();
            });
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
