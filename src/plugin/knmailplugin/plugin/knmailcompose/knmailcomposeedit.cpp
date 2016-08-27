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
#include <QLineEdit>
#include <QFontComboBox>
#include <QAbstractItemView>
#include <QComboBox>

#include "knopacitypressedbutton.h"
#include "knthememanager.h"

#include "knmailcomposeedit.h"

#include <QDebug>

#define ButtonSize 16
#define ToolBarHeight 20
#define DefaultFontSize 12

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
    //Update the text font.
    QFont defaultTextFont=font();
    defaultTextFont.setPointSize(DefaultFontSize);
    setFont(defaultTextFont);
    //Construct the button.
    for(int i=0; i<ControlButtonCount; ++i)
    {
        //Construct the button.
        m_statusButton[i]=new KNOpacityPressedButton(this);
        //Update the button status.
        m_statusButton[i]->setFixedSize(ButtonSize, ButtonSize);
        m_statusButton[i]->setCheckable(true);
        m_statusButton[i]->setOpacity(0.3);
    }
    //Set the icon.
    m_statusButton[ButtonBold]->setIcon(
                QIcon(":/plugin/mail/composer/bold.png"));
    m_statusButton[ButtonItalic]->setIcon(
                QIcon(":/plugin/mail/composer/italic.png"));
    m_statusButton[ButtonUnderline]->setIcon(
                QIcon(":/plugin/mail/composer/underline.png"));

    //Set layout to toolbar.
    QBoxLayout *toolBarLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_toolBar);
    toolBarLayout->setContentsMargins(0,0,0,0);
    m_toolBar->setLayout(toolBarLayout);
    //Add widget to layout.
    //  Font box.
    m_fontBox->view()->setFocusProxy(this);
    m_fontBox->lineEdit()->setText(defaultTextFont.family());
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
    m_fontSizeBox->lineEdit()->setText(
                QString::number(defaultTextFont.pointSize()));
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
    // Bold button.
    connect(m_statusButton[ButtonBold], &KNOpacityPressedButton::clicked,
            [=](bool has){setFontWeight(has?QFont::Bold:QFont::Normal);});
    // Italic button.
    connect(m_statusButton[ButtonItalic], &KNOpacityPressedButton::clicked,
            [=](bool has){setFontItalic(has);});
    // Underline button.
    connect(m_statusButton[ButtonUnderline], &KNOpacityPressedButton::clicked,
            [=](bool has){setFontUnderline(has);});
    //Add the status update.
    connect(this, &KNMailComposeEdit::cursorPositionChanged,
            [=]{
        //Get the current font.
        QFont textFont=currentFont();
        //Block the status widgets.
        setBlockStatus(true);
        //Update the font status.
        m_fontBox->lineEdit()->setText(textFont.family());
        m_fontSizeBox->lineEdit()->setText(
                    QString::number(textFont.pointSize()));
        m_statusButton[ButtonBold]->setChecked(fontWeight()==QFont::Bold);
        m_statusButton[ButtonItalic]->setChecked(fontItalic());
        m_statusButton[ButtonUnderline]->setChecked(fontUnderline());
        //Enable the status widget.
        setBlockStatus(false);
    });
    //Add button to layout.
    for(int i=0; i<ControlButtonCount; ++i)
    {
        //Add widget.
        toolBarLayout->addWidget(m_statusButton[i]);
    }
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

    //Link the theme change.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KNMailComposeEdit::onThemeChanged);
    onThemeChanged();
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

void KNMailComposeEdit::onThemeChanged()
{
    //Get the combo palette.
    QPalette pal=knTheme->getPalette("MailViewerCombo");
    m_fontBox->setPalette(pal);
    m_fontSizeBox->setPalette(pal);
}

void KNMailComposeEdit::setBlockStatus(bool enabled)
{
    m_fontBox->blockSignals(enabled);
    m_fontSizeBox->blockSignals(enabled);
    m_statusButton[ButtonBold]->blockSignals(enabled);
    m_statusButton[ButtonItalic]->blockSignals(enabled);
    m_statusButton[ButtonUnderline]->blockSignals(enabled);
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
