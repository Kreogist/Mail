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
#ifndef KMUNIBAR_H
#define KMUNIBAR_H

#include "kmunibarbase.h"

class KMTitleBarCombo;
/*!
 * \brief The KMUnibar class is a official realize of the title bar widget. All
 * the widget in this unibar will use SDK classes.
 */
class KMUnibar : public KMUnibarBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMUnibar widget.
     * \param parent The parent widget.
     */
    explicit KMUnibar(QWidget *parent = 0);

public slots:
    /*!
     * \brief Reimplemented from KMUnibarBase::setTitleBar().
     */
    void setTitleBar(KMTitleBarBase *titleBar) Q_DECL_OVERRIDE;

private:
    KMTitleBarCombo *m_titleCombo;
};

#endif // KMUNIBAR_H
