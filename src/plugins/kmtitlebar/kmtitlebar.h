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
#ifndef KMTITLEBAR_H
#define KMTITLEBAR_H

#include "kmtitlebarbase.h"

class KMTitleBarCombo;
class KMTitleBarButton;
/*!
 * \brief The KMTitleBar class is the default title bar realized provided
 * official.
 */
class KMTitleBar : public KMTitleBarBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMTitleBar widget.
     * \param parent The parent widget pointer.
     */
    explicit KMTitleBar(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KMTitleBarBase::setTitleText().
     */
    void setTitleText(const QString &titleText) Q_DECL_OVERRIDE;

private:
    inline KMTitleBarButton *generateButton(const char *path);
    KMTitleBarCombo *m_titleCombo;
    KMTitleBarButton *m_create, *m_settings;
};

#endif // KMTITLEBAR_H
