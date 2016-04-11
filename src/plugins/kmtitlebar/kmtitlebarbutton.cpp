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
#include <QPainter>
#include <QTimeLine>

#include "knthememanager.h"

#include "kmtitlebarbutton.h"

#define TitleBarHeight 40
#define Spacing 10
#define MaximumBrightness 110
#define MinimumBrightness 61

QLinearGradient KMTitleBarButton::m_shadowGradient=
        QLinearGradient(0, 0, 0, TitleBarHeight);
bool KMTitleBarButton::m_initial=false;

KMTitleBarButton::KMTitleBarButton(QWidget *parent) :
    QAbstractButton(parent),
    m_highlight(QLinearGradient(0, 0, 0, TitleBarHeight)),
    m_dropShadow(QColor(MinimumBrightness,
                        MinimumBrightness,
                        MinimumBrightness)),
    m_mouseInOut(new QTimeLine(200, this)),
    m_brightness(MinimumBrightness),
    m_pressed(false)
{
    setObjectName("TitleBarButton");
    //Set the fixed height of the title bar combo.
    setAutoFillBackground(true);
    //Configure the time line.
    m_mouseInOut->setUpdateInterval(33);
    m_mouseInOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KMTitleBarButton::onActionMouseInOut);

    //Check the initial flag.
    if(!m_initial)
    {
        //Reset initial flag.
        m_initial=true;
        //Initial the gradient color.
        m_shadowGradient.setColorAt(0, QColor(0,0,0,180));
        m_shadowGradient.setColorAt(1, QColor(0,0,0,0));
    }

    //Link the theme signal.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KMTitleBarButton::onActionThemeChange);
    //Update the palette.
    onActionThemeChange();
}

void KMTitleBarButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial the paint event.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw background.
    painter.fillRect(rect(), m_highlight);
    //Check the pressed.
    if(m_pressed)
    {
        //Draw the shadow.
        painter.fillRect(rect(), m_shadowGradient);
    }
    //Draw the user icon.
    if(!icon().isNull())
    {
        //Display the icon.
        painter.setOpacity((qreal)m_brightness/MaximumBrightness);
        painter.drawPixmap(Spacing, Spacing,
                           icon().pixmap(width()-(Spacing<<1),
                                         height()-(Spacing<<1)));
        painter.setOpacity(1.0);
    }
    //Disable antialiasing.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, false);
    //Draw the shadow
    painter.setPen(m_dropShadow);
    //Draw the splitter shadow.
    painter.drawLine(1, 0, 1, height());
    painter.drawLine(width()-1, 0, width()-1, height()-2);
    //Draw the bottom shadow.
    painter.drawLine(0, TitleBarHeight-1, width(), TitleBarHeight-1);

    //Draw the splitter.
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawLine(0, 0, 0, height()-2);
    //Draw the bottom.
    painter.drawLine(0, TitleBarHeight-2, width(), TitleBarHeight-2);
}

void KMTitleBarButton::enterEvent(QEvent *event)
{
    //Do original event.
    QAbstractButton::enterEvent(event);
    //Start anime.
    startAnime(MaximumBrightness);
}

void KMTitleBarButton::leaveEvent(QEvent *event)
{
    //Do original event.
    QAbstractButton::leaveEvent(event);
    //Start the anime.
    startAnime(MinimumBrightness);
}

void KMTitleBarButton::mousePressEvent(QMouseEvent *event)
{
    //Do original event.
    QAbstractButton::mousePressEvent(event);
    //Make shadow visible.
    m_pressed=true;
    //Update the button.
    update();
}

void KMTitleBarButton::mouseReleaseEvent(QMouseEvent *event)
{
    //Do original event.
    QAbstractButton::mouseReleaseEvent(event);
    //Make shadow visible.
    m_pressed=false;
    //Update the button.
    update();
}

void KMTitleBarButton::onActionThemeChange()
{
    //Update the palette.
    setPalette(knTheme->getPalette(objectName()));
    //Initial the palette.
    onActionMouseInOut(MinimumBrightness);
}

void KMTitleBarButton::onActionMouseInOut(int frame)
{
    //Change the alpha of the window text.
    QPalette pal=palette();
    //Set the high light color.
    QColor windowColor=pal.color(QPalette::Window);
    windowColor.setHsv(windowColor.hue(),
                       windowColor.saturation(),
                       frame+10);
    m_highlight.setColorAt(0, windowColor);
    windowColor.setHsl(windowColor.hue(),
                       windowColor.saturation(),
                       (qreal)frame*0.57+8.1);
    m_highlight.setColorAt(1, windowColor);
    //Set the text color.
    windowColor=pal.color(QPalette::Text);
    windowColor.setHsv(windowColor.hue(),
                       windowColor.saturation(),
                       (qreal)frame*1.8+48);
    pal.setColor(QPalette::Text, windowColor);
    setPalette(pal);

    //Update drop shadow color.
    m_brightness=frame;
    //Update the widget.
    update();
}

inline void KMTitleBarButton::startAnime(int targetBrightness)
{
    //Stop the previous anime.
    m_mouseInOut->stop();
    //Configure & Launch the anime.
    m_mouseInOut->setFrameRange(m_brightness, targetBrightness);
    //Launch the mouse in and out.
    m_mouseInOut->start();
}
