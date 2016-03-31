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

#include "kmtitlebarbutton.h"
#include "kmtitlebarcombo.h"

#include "kmtitlebar.h"

KMTitleBar::KMTitleBar(QWidget *parent) :
    KMTitleBarBase(parent),
    m_titleCombo(new KMTitleBarCombo(this)),
    m_create(generateButton("://image/public/create.png")),
    m_settings(generateButton("://image/public/settings.png"))
{
    //Set properties.
    setFixedHeight(m_titleCombo->height());

    //Configure the main layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                          this);
    //Reconfigure the main layout.
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    //Set the layout to title bar.
    setLayout(mainLayout);
    //Add widget to main layout.
    mainLayout->addWidget(m_titleCombo, 1);
    mainLayout->addWidget(m_create);
    mainLayout->addWidget(m_settings);
}

inline KMTitleBarButton *KMTitleBar::generateButton(const char *path)
{
    //Generate the button.
    KMTitleBarButton *button=new KMTitleBarButton(this);
    //Set the icon.
    button->setIcon(QIcon(QString(path)));
    button->setFixedSize(m_titleCombo->height(), m_titleCombo->height());
    //Give back the button,
    return button;
}
