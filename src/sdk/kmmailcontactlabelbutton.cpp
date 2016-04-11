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

#include "kmmailcontactlabelbutton.h"

#define ContactButtonHeight 27
#define ContactTextSpacing 5
#define ContactRoundRadius 4

KMMailContactLabelButton::KMMailContactLabelButton(QWidget *parent) :
    QAbstractButton(parent)
{
    //Set properties.
    setFixedHeight(ContactButtonHeight);
}

void KMMailContactLabelButton::paintEvent(QPaintEvent *event)
{
    //Ignore the event.
    Q_UNUSED(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the background border.
    painter.setPen(palette().color(QPalette::ButtonText));
    painter.setBrush(palette().color(QPalette::Button));
    painter.drawRoundedRect(QRect(0,0,width(),height()),
                            ContactRoundRadius, ContactRoundRadius);
    //Prepare the text X position.
    int textX=ContactTextSpacing;
    //Draw the icon and label.
    //Check whether the icon is valid.
    if(!m_avatarIcon.isNull())
    {
        //Draw the avatar icon.
        painter.drawPixmap(0, 0, m_avatarIcon);
        //Move the text X.
        textX+=ContactButtonHeight;
        //Draw the splitter.
        painter.setRenderHints(QPainter::Antialiasing, false);
        painter.drawLine(ContactButtonHeight, 0,
                         ContactButtonHeight, ContactButtonHeight);
        painter.setRenderHints(QPainter::Antialiasing, true);
    }
    //Draw the text.
    painter.drawText(QRect(textX,
                           0,
                           width(),
                           ContactButtonHeight),
                     Qt::AlignVCenter,
                     text());
    //Draw the border.
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(QRect(0,0,width(),height()),
                            ContactRoundRadius, ContactRoundRadius);
}

QPixmap KMMailContactLabelButton::avatarIcon() const
{
    return m_avatarIcon;
}

QSize KMMailContactLabelButton::sizeHint() const
{
    return QSize((m_avatarIcon.isNull() ? 0 : ContactButtonHeight) +
                 (ContactTextSpacing<<1) + fontMetrics().width(text()),
                 ContactButtonHeight);
}

void KMMailContactLabelButton::setText(const QString &text)
{
    //Update the text.
    QAbstractButton::setText(text);
    //Update the minimum size.
    QSize buttonSize=sizeHint();
    //Update the button size.
    setMinimumSize(buttonSize);
    setFixedSize(buttonSize);
}

void KMMailContactLabelButton::setAvatarIcon(const QPixmap &avatarIcon)
{
    //Check the validation of the avatar icon.
    if(avatarIcon.isNull())
    {
        //Clear the avatar icon.
        m_avatarIcon=QPixmap();
    }
    else
    {
        //Reset the avatar icon.
        m_avatarIcon=QPixmap(ContactButtonHeight, ContactButtonHeight);
        //Rescaled the icon.
        QPixmap icon=avatarIcon.scaled(ContactButtonHeight,
                                       ContactButtonHeight,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
        //Paint icon on the avatar icon data.
        m_avatarIcon.fill(QColor(255, 255, 255, 0));
        //Painter the icon.
        QPainter painter(&m_avatarIcon);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        //Draw the content border.
        painter.setBrush(QColor(255, 255, 255));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(QRect(0,
                                      0,
                                      ContactButtonHeight<<1,
                                      ContactButtonHeight),
                                ContactRoundRadius, ContactRoundRadius);
        //Draw the icon on the avatar.
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        painter.drawPixmap((ContactButtonHeight - icon.width())>>1,
                           (ContactButtonHeight - icon.height())>>1,
                           icon);
        //Close the painter.
        painter.end();
    }
    //Update the minimum size.
    QSize buttonSize=sizeHint();
    //Update the button size.
    setMinimumSize(buttonSize);
    setFixedSize(buttonSize);
    //Rerendering the button.
    update();
}
