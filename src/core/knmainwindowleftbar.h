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
#ifndef KNMAINWINDOWLEFTBAR_H
#define KNMAINWINDOWLEFTBAR_H

#include "knmainwindowleftbarbase.h"

class QBoxLayout;
/*!
 * \brief The KNMainWindowLeftBar class is default realize of the main window
 * left bar.
 */
class KNMainWindowLeftBar : public KNMainWindowLeftBarBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMainWindowLeftBar widget.
     * \param parent The parent widget.
     */
    explicit KNMainWindowLeftBar(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMainWindowLeftBarBase::addLeftBarWidget().
     */
    void addLeftBarWidget(QWidget *widget,
                          int stretch = 0,
                          Qt::Alignment alignment = 0) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    QBoxLayout *m_mainLayout;
};

#endif // KNMAINWINDOWLEFTBAR_H
