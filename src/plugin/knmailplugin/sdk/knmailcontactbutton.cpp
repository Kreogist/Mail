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

#include "knmailcontactbutton.h"

#define SideSpacing 2
#define FontSize 12
#define ButtonRadius 12

KNMailContactButton::KNMailContactButton(QWidget *parent) :
    QWidget(parent),
    m_avatar(QPixmap()),
    m_caption(QString()),
    m_address(QString()),
    m_targetText(QString())
{
    //Set properties.
    setFixedHeight(ButtonHeight);
    //Configure the font size.
    QFont textFont=font();
    textFont.setPixelSize(FontSize);
    setFont(textFont);
}

void KNMailContactButton::setContact(const QString &caption,
                                      const QString &address,
                                      const QPixmap &avatar)
{
    //Save the caption and address.
    m_caption=caption;
    m_address=address;
    //Save the avatar.
    m_avatar=avatar;
    //Check avatar validation.
    if(m_avatar.isNull())
    {
        //Load avatar from contact.
        //! FIXME: add search avatar from contact codes here.
    }
    //Check the caption.
    if(m_caption.isEmpty())
    {
        //Load the caption form contact.
        //! FIXME: add check caption from contact codes here.
        //Check the the caption data.
        if(m_caption.isEmpty())
        {
            //Use the E-mail title name as the text.
            int atPosition=address.indexOf('@');
            //Check the position.
            if(atPosition>-1)
            {
                //Cut off the caption.
                m_caption=address.left(atPosition);
            }
        }
    }
    //Construct the content text.
    m_contentText=m_caption + " <" + m_address + ">";
    //We have to ensure that the length
    //After load the avatar, calculate the button size.
    m_targetWidth=fontMetrics().width(m_contentText) + ButtonHeight +
            (m_avatar.isNull()?0:ButtonHeight);
    //Check the possible width is larger or smaller than this one.
    updateWidth();
    //Update the widget.
    update();
}

void KNMailContactButton::updateWidth()
{
    //Get the valid width.
    int availableWidth=parentWidget()->width()-
            (m_avatar.isNull()?0:ButtonHeight);
    //Check out the target width is larger than our target width.
    if(m_targetWidth>availableWidth)
    {
        //Update the button's width.
        setFixedWidth(availableWidth);
        //Update the target text.
        m_targetText=fontMetrics().elidedText(m_contentText,
                                              Qt::ElideRight,
                                              availableWidth -
                                              (ButtonRadius<<1) -
                                              (SideSpacing));
        //Mission complete.
        return;
    }
    //Update the button's size to target size.
    setFixedWidth(m_targetWidth);
    //Update the target text.
    m_targetText=m_contentText;
}

void KNMailContactButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the base.
    const QPalette &pal=palette();
    QPen basePen;
    basePen.setColor(pal.color(QPalette::WindowText));
    painter.setPen(basePen);
    painter.setBrush(pal.color(QPalette::Window));
    //Draw the avatar and the text.
    painter.drawRoundedRect(QRect(2, 0, width()-4, height()),
                            ButtonRadius,
                            ButtonRadius);
    //Draw the contents.
    painter.setPen(pal.color(QPalette::ButtonText));
    painter.drawText(QRect((m_avatar.isNull()?
                                ButtonRadius : (ButtonRadius+ButtonHeight)),
                           0,
                           width()-ButtonHeight,
                           height()),
                     Qt::AlignVCenter,
                     m_targetText);
}
