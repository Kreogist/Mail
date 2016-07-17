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
#ifndef KNMAILVIEWER_H
#define KNMAILVIEWER_H

#include "knmailviewerbase.h"

class QLabel;
/*!
 * \brief The KNMailViewer class provides a default realize of the mail viewer.
 * This viewer could be popup and view the mail in a new widget.
 */
class KNMailViewer : public KNMailViewerBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailViewer widget.
     * \param parent The parent widget.
     */
    explicit KNMailViewer(QWidget *parent = 0);

signals:

public slots:

private:
    QLabel *m_subject;
};

#endif // KNMAILVIEWER_H
