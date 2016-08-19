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

#ifndef KNMAILWEBVIEWERGENERATORBASE_H
#define KNMAILWEBVIEWERGENERATORBASE_H

class QWidget;
class KNMailWebViewerBase;
/*!
 * \brief The KNMailWebViewerGeneratorBase class provides the basic ports of a
 * virtual factory to generate a web viewer widget. All the web viewer widget
 * should provide a generator object for itself. The generator will be stored in
 * mail global class.
 */
class KNMailWebViewerGeneratorBase
{
public:
    /*!
     * \brief Construct a KNMailWebViewerGeneratorBase object.
     */
    KNMailWebViewerGeneratorBase(){}
    virtual ~KNMailWebViewerGeneratorBase(){}

    /*!
     * \brief Generate a web viewer widget with specific parent widget.
     * \param parent The parent widget pointer.
     * \return The viewer widget pointer.
     */
    virtual KNMailWebViewerBase *generateWebViewer(QWidget *parent)=0;
};

#endif // KNMAILWEBVIEWERGENERATORBASE_H
