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

#include "knmailviewer.h"

#include "knmailviewergenerator.h"

KNMailViewerBase *KNMailViewerGenerator::generateViewer(QWidget *parent)
{
    //Generate the viewer.
    return new KNMailViewer(parent);
}

KNMailViewerBase *KNMailViewerGenerator::generatePopupViewer(QWidget *parent)
{
    //Generate the viewer.
    KNMailViewer *viewer=new KNMailViewer(parent);
    //Configure the viewer.
    viewer->setViewerPopup(true);
    //Set the mime viewer.
    viewer->setMinimumSize(853, 480);
    //Give back the viewer.
    return viewer;
}