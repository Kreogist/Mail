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
#include <QLinearGradient>

#include "knthememanager.h"

#include "kmtitlebarcombo.h"

#define TitleBarHeight 40
#define Spacing 16
#define MaximumBrightness 110
#define MinimumBrightness 61

KMTitleBarCombo::KMTitleBarCombo(QWidget *parent) :
    QAbstractButton(parent),
    m_highlight(QLinearGradient(0, 0, 0, TitleBarHeight)),
    m_shadowGradient(QLinearGradient(0, 0, 0, TitleBarHeight)),
    m_userAvatar(QPixmap()),
    m_anonymous(QPixmap("://image/public/anonymous.png").scaled(
                    TitleBarHeight,
                    TitleBarHeight,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation)),
    m_indicator(QPixmap("://image/public/AscendingIndicator.png")),
    m_dropShadow(QColor(MinimumBrightness,
                        MinimumBrightness,
                        MinimumBrightness)),
    m_text(QString()),
    m_mouseInOut(new QTimeLine(200, this)),
    m_brightness(MinimumBrightness),
    m_indicatorRotate(0),
    m_pressed(false)
{
    setObjectName("TitleBarCombo");
    //Set the fixed height of the title bar combo.
    setAutoFillBackground(true);
    setMinimumWidth(TitleBarHeight+(Spacing<<1));
    setFixedHeight(TitleBarHeight);
    QFont titleFont=font();
    titleFont.setPixelSize(Spacing);
    setFont(titleFont);
    //Update the gradient.
    m_shadowGradient.setColorAt(0, QColor(0,0,0,180));
    m_shadowGradient.setColorAt(1, QColor(0,0,0,0));

    //Configure the time line.
    m_mouseInOut->setUpdateInterval(33);
    m_mouseInOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_mouseInOut, &QTimeLine::frameChanged,
            this, &KMTitleBarCombo::onActionMouseInOut);

    //Link the theme signal.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KMTitleBarCombo::onActionThemeChange);
    //Update the palette.
    onActionThemeChange();
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
    QAbstractButton::enterEvent(event);
    //Start anime when enabled.
    if(isEnabled())
    {
        //Launch the animation.
        startAnime(MaximumBrightness);
    }
}

void KMTitleBarCombo::leaveEvent(QEvent *event)
{
    //Do original event.
    QAbstractButton::leaveEvent(event);
    //Start anime when enabled.
    if(isEnabled())
    {
        //Launch the animation.
        startAnime(MinimumBrightness);
    }
}

void KMTitleBarCombo::paintEvent(QPaintEvent *event)
{
    //Draw all the other things.
    Q_UNUSED(event)
    //Initial the painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Draw background.
    painter.fillRect(rect(), m_highlight);
    //Check whether is pressed.
    if(m_pressed)
    {
        //Paint the shadow.
        painter.fillRect(rect(), m_shadowGradient);
    }
    //Draw the user avatar.
    painter.drawPixmap(0, 0, m_userAvatar.isNull()?m_anonymous:m_userAvatar);
    //Configure the pen.
    QPen borderPen;
    borderPen.setColor(palette().color(QPalette::Text));
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
    //Draw the indicator.
    painter.save();
    //Move to indicator position.
    painter.translate(width()-(Spacing>>1)-(m_indicator.width()>>1),
                      TitleBarHeight>>1);
    painter.rotate(m_indicatorRotate);
    painter.translate(-m_indicator.width()>>1, -m_indicator.height()>>1);
    painter.drawPixmap(QRect(QPoint(0, 0), m_indicator.size()),
                       m_indicator);
    painter.restore();

    //Disable antialiasing.
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, false);
    //Draw the shadow
    painter.setPen(m_dropShadow);
    //Draw the bottom shadow.
    painter.drawLine(0, TitleBarHeight-1, width(), TitleBarHeight-1);
    painter.drawLine(width()-1, 0, width()-1, height());
    //Draw the splitter shadow.
    painter.setPen(QColor(m_brightness, m_brightness, m_brightness));
    painter.drawLine(TitleBarHeight+2, 0, TitleBarHeight+2, height());

    //Draw the splitter.
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawLine(TitleBarHeight+1, 0, TitleBarHeight+1, height()-2);
    //Draw the bottom.
    painter.drawLine(0, TitleBarHeight-2, width(), TitleBarHeight-2);
}

void KMTitleBarCombo::mousePressEvent(QMouseEvent *event)
{
    //Do the press event.
    QAbstractButton::mousePressEvent(event);
    //Check the enabled.
    if(isEnabled())
    {
        //Update the state.
        m_pressed=true;
        //Update the widget.
        update();
    }
}

void KMTitleBarCombo::mouseReleaseEvent(QMouseEvent *event)
{
    //Do the release event.
    QAbstractButton::mouseReleaseEvent(event);
    //Check the enabled.
    if(isEnabled())
    {
        //Update the state.
        m_pressed=false;
        //Update the widget.
        update();
    }
}

void KMTitleBarCombo::onActionThemeChange()
{
    //Update the palette.
    setPalette(knTheme->getPalette(objectName()));
    //Initial the palette.
    onActionMouseInOut(MinimumBrightness);
}

void KMTitleBarCombo::onActionMouseInOut(int frame)
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

    //Update the brightness.
    m_brightness=frame;
    //Update the widget.
    update();
}

inline void KMTitleBarCombo::startAnime(int targetBrightness)
{
    //Stop the previous anime.
    m_mouseInOut->stop();
    //Configure & Launch the anime.
    m_mouseInOut->setFrameRange(m_brightness, targetBrightness);
    //Launch the mouse in and out.
    m_mouseInOut->start();
}

int KMTitleBarCombo::indicatorRotate() const
{
    return m_indicatorRotate;
}

void KMTitleBarCombo::setIndicatorRotate(int indicatorRotate)
{
    //Save the angle.
    m_indicatorRotate = indicatorRotate;
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
