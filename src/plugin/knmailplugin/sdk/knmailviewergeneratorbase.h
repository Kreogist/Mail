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

#ifndef KNMAILVIEWERGENERATORBASE_H
#define KNMAILVIEWERGENERATORBASE_H

class QWidget;
class KNMailViewerBase;
/*!
 * \brief The KNMailViewerGeneratorBase class provides the basic ports of a mail
 *  viewer generator should be porvided.\n
 * A generator class should be provide via the viewer, and it will be installed
 * into the mail global class for supporting popup manager.
 */
class KNMailViewerGeneratorBase
{
public:
    /*!
     * \brief Construct a KNMailViewerGeneratorBase object.
     */
    KNMailViewerGeneratorBase(){}
    virtual ~KNMailViewerGeneratorBase(){}

    /*!
     * \brief Generate an mail viewer widget.
     * \param parent The parent widget.
     * \return The generated mail viewer pointer.
     */
    virtual KNMailViewerBase *generateViewer(QWidget *parent)=0;

    /*!
     * \brief Generate a popup mail viewer widget.
     * \param parent The parent widget.
     * \return The viewer pointer.
     */
    virtual KNMailViewerBase *generatePopupViewer(QWidget *parent)=0;
};

#endif // KNMAILVIEWERGENERATORBASE_H
