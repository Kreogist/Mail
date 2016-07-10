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
#ifndef KNACCOUNTWIDGET_H
#define KNACCOUNTWIDGET_H

#include "knhwidgetswitcher.h"

/*!
 * \brief The KNAccountWidget class provides a widget container for the Kreogist
 * Account system for Kreogist Mail application. It should be used with the
 * account object.\n
 * It is a widget like plugin manager, it will control those panels according to
 * the signals. It works like a finite state machine.
 */
class KNAccountWidget : public KNHWidgetSwitcher
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNAccountWidget widget.
     * \param parent The parent widget pointer.
     */
    explicit KNAccountWidget(QWidget *parent = 0);

signals:

public slots:

private slots:
};

#endif // KNACCOUNTWIDGET_H
