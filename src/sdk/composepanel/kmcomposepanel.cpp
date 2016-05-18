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
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>

//Dependencies.
#include "knlocalemanager.h"
#include "knthememanager.h"

//Compose panel widgets.
#include "kmcomposelineedit.h"
#include "kmcomposeeditor.h"

#include "kmcomposepanel.h"

KMComposePanel::KMComposePanel(QWidget *parent) :
    QWidget(parent),
    m_from(new QComboBox(this)),
    m_title(new KMComposeLineEdit(this)),
    m_receiver(new KMComposeLineEdit(this)),
    m_fromHint(new QLabel(this)),
    m_toHint(new QLabel(this)),
    m_editor(new KMComposeEditor(this))
{
    setObjectName("ComposePanel");
    //Set properties.
    setAutoFillBackground(true);
    knTheme->registerWidget(this);
    //Initial the line edits.
    m_title->setObjectName("ComposeLineEdit");
    knTheme->registerWidget(m_title);
    QFont titleFont=m_title->font();
    titleFont.setPixelSize(20);
    m_title->setFont(titleFont);
    m_receiver->setObjectName("ComposeLineEdit");
    knTheme->registerWidget(m_receiver);
    //Update the hint labels.
    m_fromHint->setObjectName("ComposeLabel");
    m_toHint->setObjectName("ComposeLabel");
    knTheme->registerWidget(m_fromHint);
    knTheme->registerWidget(m_toHint);
    //Configure the combo settings.
    m_from->setObjectName("ComposeCombo");
    knTheme->registerWidget(m_from);
    //Initial the widget as dialog.
    setWindowFlags(Qt::Dialog);

    //Initial the layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    setLayout(mainLayout);
    //Add widget to main layout.
    mainLayout->addWidget(m_title);
    //Initial the form layout.
    QFormLayout *receiverForm=new QFormLayout();
    //Update the layout.
    receiverForm->setLabelAlignment(Qt::AlignTop | Qt::AlignRight);
    //Add widget to layout.
    receiverForm->addRow(m_fromHint, m_from);
    receiverForm->addRow(m_toHint, m_receiver);
    //Add to main layout.
    mainLayout->addLayout(receiverForm);
    //Add editor to main layout.
    mainLayout->addWidget(m_editor, 1);

    //Link the locale stuffs.
    knI18n->link(this, &KMComposePanel::retranslate);
    retranslate();

    //Debug.
    m_from->addItem("tojo.nozomi@ll-anime.com");
}

void KMComposePanel::retranslate()
{
    //Translate the line edit place holder text.
    m_title->setPlaceholderText(tr("Title"));
    //Update the label hints.
    m_fromHint->setText(tr("From: "));
    m_toHint->setText(tr("To: "));
}
