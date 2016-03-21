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
#include <QApplication>
#include <QPainter>
#include <QTimeLine>

#include "knthememanager.h"

#include "kmtitlebarcombo.h"

#define TitleBarHeight 36
#define Spacing 5
#define MaximumBrightness 0xA0
#define MinimumBrightness 0x20

KMTitleBarCombo::KMTitleBarCombo(QWidget *parent) :
    QWidget(parent),
    m_userAvatar(QPixmap()),
    m_anonymous(QPixmap("://image/public/anonymous.png").scaled(
                    TitleBarHeight, TitleBarHeight,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation)),
    m_text(QString()),
    m_mouseInOut(new QTimeLine(200, this))
{
    setObjectName("TitleBarCombo");
    //Set the fixed height of the title bar combo.
    setFixedHeight(TitleBarHeight);
    setMinimumWidth(100);

    //Configure the time line.
    m_mouseInOut->setUpdateInterval(33);
    m_mouseInOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KMTitleBarCombo::onActionMouseInOut);

    //Add to theme manager.
    knTheme->registerWidget(this);
}

QString KMTitleBarCombo::text() const
{
    return m_text;
}

void KMTitleBarCombo::setText(const QString &text)
{
    m_text = text;
}

void KMTitleBarCombo::enterEvent(QEvent *event)
{
    //Do original event.
    QWidget::enterEvent(event);
    //Configure & Launch the anime.
    m_mouseInOut->setFrameRange(palette().color(QPalette::WindowText).alpha(),
                                MaximumBrightness);
    //Launch the mouse in and out.
    m_mouseInOut->start();
}

void KMTitleBarCombo::leaveEvent(QEvent *event)
{
    //Do original event.
    QWidget::leaveEvent(event);
    //Configure & Launch the anime.
    m_mouseInOut->setFrameRange(palette().color(QPalette::WindowText).alpha(),
                                MinimumBrightness);
    //Launch the mouse in and out.
    m_mouseInOut->start();
}

void KMTitleBarCombo::paintEvent(QPaintEvent *event)
{
    //Draw all the other things.
    QWidget::paintEvent(event);
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw the user avatar.
    painter.drawPixmap(0, 0, m_userAvatar.isNull()?m_anonymous:m_userAvatar);
    //Configure the pen.
    QPen borderPen;
    borderPen.setColor(palette().color(QPalette::WindowText));
    borderPen.setWidth(1);
    painter.setPen(borderPen);
    //Draw the text.
    painter.drawText(QRect(TitleBarHeight+Spacing,
                           0,
                           width()-TitleBarHeight-(Spacing<<1),
                           TitleBarHeight),
                     Qt::AlignVCenter,
                     m_text.isEmpty()?
                         QApplication::applicationName():
                         m_text);
    //Draw the splitter.
    painter.drawRect(QRect(-1, -1, TitleBarHeight+2, TitleBarHeight+2));
    //Draw the bottom.
    painter.drawLine(QPoint(0, TitleBarHeight-1),
                     QPoint(width(), TitleBarHeight-1));
}

void KMTitleBarCombo::onActionMouseInOut(int frame)
{
    //Change the alpha of the window text.
    QPalette pal=palette();
    //Set the text color.
    QColor textColor=pal.color(QPalette::WindowText);
    textColor.setAlpha(frame);
    //Change the palette.
    pal.setColor(QPalette::WindowText, textColor);
    //Set back the palette.
    setPalette(pal);
    //Update the widget.
    update();
}

QPixmap KMTitleBarCombo::userAvatar() const
{
    return m_userAvatar;
}

void KMTitleBarCombo::setUserAvatar(const QPixmap &userAvatar)
{
    //Scaled the user avatar just in fit.
    m_userAvatar = userAvatar.isNull()?
                QPixmap():
                userAvatar.scaled(TitleBarHeight,
                                  TitleBarHeight,
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation);
}
