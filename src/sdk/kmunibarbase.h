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
#ifndef KMUNIBARBASE_H
#define KMUNIBARBASE_H

#include "mailaccount/kmmailaccount.h"

#include <QWidget>

class KMTitleBarBase;
/*!
 * \brief The KMUnibarBase class provides all the ports that a unibar widget
 * should be provided.\n
 * The unibar will be used on main window for switching different list and
 * account.
 */
class KMUnibarBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMUnibarBase widget.
     * \param parent The parent widget.
     */
    KMUnibarBase(QWidget *parent = 0) : QWidget(parent){}

    /*!
     * \brief Set shadow parent widget.
     * \param container The shadow parent widget.
     */
    virtual void setShadowParent(QWidget *container)=0;

signals:
    /*!
     * \brief switchModel
     * \param account
     * \param modelIndex
     */
    void switchModel(KMMailAccount *account,
                     int modelIndex);

    /*!
     * \brief requireUpdateTitle
     * \param titleText
     */
    void requireUpdateTitle(QString titleText);

public slots:
    /*!
     * \brief Set the title bar widget to unibar.
     * \param titleBar The title bar widget pointer.
     */
    virtual void setTitleBar(KMTitleBarBase *titleBar)=0;

    /*!
     * \brief Show unibar by given the main window size.
     * \param mainWindowSize The main window size.
     */
    virtual void showUnibar(QSize mainWindowSize)=0;

    /*!
     * \brief Hide the unibar widget.
     */
    virtual void hideUnibar()=0;
};

#endif // KMUNIBARBASE_H
