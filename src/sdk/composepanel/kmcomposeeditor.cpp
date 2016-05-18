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
#include <QBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QFontComboBox>

#include "kmcomposebutton.h"
#include "knthememanager.h"

#include "kmcomposeeditor.h"

#include <QDebug>

#define ButtonSize 22

KMComposeEditor::KMComposeEditor(QWidget *parent) :
    QWidget(parent),
    m_fontFamily(new QFontComboBox(this)),
    m_fontSize(new QComboBox(this)),
    m_editArea(new QTextEdit(this))
{
    //Set properties.
    setContentsMargins(0, 0, 0, 0);
    //Update the font.
    QFont editFont=m_editArea->font();
    editFont.setPointSize(12);
    m_editArea->setFont(editFont);
    //Initial buttons.
    initialTools();
    //Configure the font family combo box.
    connect(m_fontFamily->lineEdit(), &QLineEdit::textChanged,
            this, &KMComposeEditor::onActionChangeFont);
    //Configure the font size combo box.
    m_fontSize->setEditable(true);
    connect(m_fontSize->lineEdit(), &QLineEdit::textChanged,
            this, &KMComposeEditor::onActionChangeFontSize);
    //Configure the edia area.
    connect(m_editArea, &QTextEdit::cursorPositionChanged,
            this, &KMComposeEditor::onCursorPositionChange);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(2);
    setLayout(mainLayout);
    //Initial the toolbox layout.
    QBoxLayout *toolbar=new QBoxLayout(QBoxLayout::LeftToRight);
    toolbar->setSpacing(2);
    mainLayout->addLayout(toolbar);
    //Add button to toolbar.
    toolbar->addWidget(m_fontFamily);
    toolbar->addWidget(m_fontSize);
    toolbar->addWidget(m_toolButton[ToolBold]);
    toolbar->addWidget(m_toolButton[ToolItalic]);
    toolbar->addWidget(m_toolButton[ToolUnderline]);
    toolbar->addWidget(m_toolButton[ToolIncreaseIndent]);
    toolbar->addWidget(m_toolButton[ToolDecreaseIndent]);
    toolbar->addStretch();

    //Add widget to the main layout.
    mainLayout->addWidget(m_editArea, 1);

    //Upadte the theme.
    connect(knTheme, &KNThemeManager::themeChange,
            this, &KMComposeEditor::onActionChangeTheme);
    onActionChangeTheme();
}

void KMComposeEditor::onCursorPositionChange()
{
    //Get the curent text cursor .
    QTextCursor textCursor=m_editArea->textCursor();
    //Get the font information.
    QTextCharFormat &&currentFormat=textCursor.charFormat();
    //Get the font from the current format.
    QFont &&currentFont=currentFormat.font();
    //Output format to the text cursor.
    updateFormatState(ToolBold, currentFont.bold());
    updateFormatState(ToolItalic, currentFont.italic());
    updateFormatState(ToolUnderline, currentFont.underline());
    //Change the font family name.
    m_fontFamily->lineEdit()->blockSignals(true);
    m_fontFamily->lineEdit()->setText(currentFont.family());
    m_fontFamily->lineEdit()->blockSignals(false);
    //Change the font size.
    m_fontSize->lineEdit()->blockSignals(true);
    m_fontSize->lineEdit()->setText(QString::number(currentFont.pointSize()));
    m_fontSize->lineEdit()->blockSignals(false);
}

void KMComposeEditor::onActionChangeTheme()
{
    //Get the button palette.
    QPalette pal=knTheme->getPalette("ComposeButton");
    //Update button palette.
    for(int i=0; i<TextEditToolCount; ++i)
    {
        //Set the palette.
        m_toolButton[i]->setPalette(pal);
    }
    //Update the combo palette.
    pal=knTheme->getPalette("ComposeCombo");
    m_fontFamily->setPalette(pal);
    m_fontFamily->lineEdit()->setPalette(pal);
    m_fontSize->setPalette(pal);
    m_fontSize->lineEdit()->setPalette(pal);
}

void KMComposeEditor::onActionChangeBold(bool has)
{
    //Update the font widget.
    m_editArea->setFontWeight(has?QFont::Bold:QFont::Normal);
    //Set focus to edit area.
    m_editArea->setFocus();
}

void KMComposeEditor::onActionChangeItalic(bool has)
{
    //Update the font widget.
    m_editArea->setFontItalic(has);
    //Set focus to edit area.
    m_editArea->setFocus();
}

void KMComposeEditor::onActionChangeUnderline(bool has)
{
    //Update the font widget.
    m_editArea->setFontUnderline(has);
    //Set focus to edit area.
    m_editArea->setFocus();
}

void KMComposeEditor::onActionChangeFont(const QString &fontFamily)
{
    //Update the font family.
    m_editArea->setFontFamily(fontFamily);
}

void KMComposeEditor::onActionChangeFontSize(const QString &fontSize)
{
    //Update the font size.
    m_editArea->setFontPointSize(fontSize.toInt());
}

inline void KMComposeEditor::initialTools()
{
    //Configure the button.
    generateButton(ToolBold, "://image/editor/bold.png");
    generateButton(ToolItalic, "://image/editor/italic.png");
    generateButton(ToolUnderline, "://image/editor/underline.png");
    generateButton(ToolIncreaseIndent, "://image/editor/indent_increase.png");
    generateButton(ToolDecreaseIndent, "://image/editor/indent_decrease.png");
    //Link the buttons.
    m_toolButton[ToolBold]->setCheckable(true);
    connect(m_toolButton[ToolBold], &KMComposeButton::clicked,
            this, &KMComposeEditor::onActionChangeBold);
    m_toolButton[ToolItalic]->setCheckable(true);
    connect(m_toolButton[ToolItalic], &KMComposeButton::clicked,
            this, &KMComposeEditor::onActionChangeItalic);
    m_toolButton[ToolUnderline]->setCheckable(true);
    connect(m_toolButton[ToolUnderline], &KMComposeButton::clicked,
            this, &KMComposeEditor::onActionChangeUnderline);
}

inline void KMComposeEditor::generateButton(int index, const QString &iconPath)
{
    //Initial the button.
    m_toolButton[index]=new KMComposeButton(this);
    //Configure the button.
    m_toolButton[index]->setFixedSize(ButtonSize, ButtonSize);
    m_toolButton[index]->setIcon(QIcon(iconPath));
}

inline void KMComposeEditor::updateFormatState(int index, bool has)
{
    //Block the button.
    m_toolButton[index]->blockSignals(true);
    //Set the checked state.
    m_toolButton[index]->setChecked(has);
    //Release the button.
    m_toolButton[index]->blockSignals(false);
}
