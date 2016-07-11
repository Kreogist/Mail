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

#include "knthememanager.h"

#include "knmailfolderviewertitle.h"

#define HeaderHeight 28
#define FontHeight 14
#define Spacing 7

KNMailFolderViewerTitle::KNMailFolderViewerTitle(QWidget *parent) :
    QWidget(parent)
{
    setObjectName("MailFolderViewerTitle");
    //Set properties.
    setFixedHeight(HeaderHeight);
    QFont titleFont=font();
    titleFont.setPixelSize(FontHeight);
    setFont(titleFont);
    //Register to theme manager.
    knTheme->registerWidget(this);

}

void KNMailFolderViewerTitle::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    //Initial painter.
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::SmoothPixmapTransform, true);
    //Fill the background.
    painter.fillRect(rect(), palette().color(QPalette::Window));
    //Check the folder name text.
    if(!m_folderName.isEmpty())
    {
        //Draw the title text.
        int textWidth=width()-(Spacing<<1);
        painter.drawText(QRect(Spacing, Spacing, textWidth, FontHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         fontMetrics().elidedText(m_folderName,
                                                  Qt::ElideRight,
                                                  textWidth));
    }
    //Draw the border.
    painter.drawLine(QPoint(0,0), QPoint(width(),0));
    painter.drawLine(QPoint(0,HeaderHeight), QPoint(width(),HeaderHeight));
}

QString KNMailFolderViewerTitle::folderName() const
{
    return m_folderName;
}

void KNMailFolderViewerTitle::setFolderName(const QString &folderName)
{
    //Save the folder name.
    m_folderName = folderName;
    //Update the widget.
    update();
}

void KNMailFolderViewerTitle::clear()
{
    //Reset the title text.
    m_folderName.clear();
    //Update the widget.
    update();
}
