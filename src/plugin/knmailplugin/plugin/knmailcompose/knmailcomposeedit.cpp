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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QBoxLayout>
#include <QTimeLine>
#include <QFontComboBox>
#include <QAbstractItemView>
#include <QComboBox>

#include "knmailcomposeedit.h"

#include <QDebug>

#define ToolBarHeight 20

KNMailComposeEdit::KNMailComposeEdit(QWidget *parent) :
    QTextEdit(parent),
    m_toolBar(new QWidget(this)),
    m_fontBox(new QFontComboBox(this)),
    m_fontSizeBox(new QComboBox(this)),
    m_toolBarAnime(new QTimeLine(200, this)),
    m_shownHeight(0)
{
    //Set properties.
    setViewportMargins(0, 0, 0, ToolBarHeight);
    //Update toolbar sets.
    m_shownHeight=ToolBarHeight;
    m_toolBar->setFixedHeight(ToolBarHeight);
    m_toolBar->setFocusProxy(this);

    //Set layout to toolbar.
    QBoxLayout *toolBarLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_toolBar);
    toolBarLayout->setContentsMargins(0,0,0,0);
    m_toolBar->setLayout(toolBarLayout);
    //Add widget to layout.
    //  Font box.
    m_fontBox->view()->setFocusProxy(this);
    toolBarLayout->addWidget(m_fontBox);
    connect(m_fontBox, &QFontComboBox::currentFontChanged,
            [=](const QFont &font){
        setFontFamily(font.family());
    });
    //  Font size box.
    m_fontSizeBox->setEditable(true);
    QStringList fontSizeList;
    fontSizeList << "9" << "10" << "11" << "12" << "13" << "14" << "18" << "24"
                 << "36" << "48" << "64" << "72" << "96" << "144" << "288";
    m_fontSizeBox->addItems(fontSizeList);
    toolBarLayout->addWidget(m_fontSizeBox);
    connect(m_fontSizeBox, &QComboBox::currentTextChanged,
            [=](const QString &fontText){
        //Calculate the font size.
        int fontSize=fontText.toInt();
        //Check the validation of the font size.
        if(fontSize<=0)
        {
            //Set the minimum font size.
            fontSize=1;
        }
        //Check the font text size.
        setFontPointSize(fontSize);
    });
    toolBarLayout->addStretch();
    //Configure the animation.
    m_toolBarAnime->setUpdateInterval(33);
    m_toolBarAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_toolBarAnime, &QTimeLine::frameChanged,
            [=](int toolBarHeight)
    {
        //Show the shown height.
        m_shownHeight=toolBarHeight;
        //Update the margin.
        setViewportMargins(0, 0, 0, m_shownHeight);
        //Move the tool bar.
        updateToolBarGeometry();
    });
}

void KNMailComposeEdit::resizeEvent(QResizeEvent *event)
{
    //Resize the widget.
    QTextEdit::resizeEvent(event);
    //Update the size.
    updateToolBarGeometry();
}

void KNMailComposeEdit::focusInEvent(QFocusEvent *event)
{
    //Start anime.
//    startAnime(ToolBarHeight);
    //Do original event.
    QTextEdit::focusInEvent(event);
}

void KNMailComposeEdit::focusOutEvent(QFocusEvent *event)
{
    //Start anime.
//    startAnime(0);
    //Do original event.
    QTextEdit::focusOutEvent(event);
}

void KNMailComposeEdit::startAnime(int endFrame)
{
    //Stop anime.
    m_toolBarAnime->stop();
    //Update frame range.
    m_toolBarAnime->setFrameRange(m_shownHeight, endFrame);
    //Start the anime.
    m_toolBarAnime->start();
}

void KNMailComposeEdit::updateToolBarGeometry()
{
    //Update the tool bar position.
    m_toolBar->setGeometry(0,
                           height()-m_shownHeight,
                           width(),
                           ToolBarHeight);
}

