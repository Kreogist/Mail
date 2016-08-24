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
#ifndef KNMAINWINDOWLEFTBARBASE_H
#define KNMAINWINDOWLEFTBARBASE_H

#include <QWidget>

/*!
 * \brief The KNMainWindowLeftBarBase class provides the basic port functions of
 * the left bar in the main window. It is an abstract class, to subclass a left
 * bar widget, you have to implement this class and realize all the virtual
 * functions.
 */
class KNMainWindowLeftBarBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMainWindowLeftBarBase widget.
     * \param parent The parent widget pointer.
     */
    KNMainWindowLeftBarBase(QWidget *parent = 0): QWidget(parent){}

    /*!
     * \brief Add a widget to left bar. There may be many widgets will be added
     * to left bar, so the header widget should provide a layout or a stacked
     * widget to store these widgets. You'd better to use a box layout.
     * \param widget The widget pointer.
     * \param stretch The widget stretch parameter. Default is 0.
     * \param alignment The widget prefer alignment. Default is 0, which means
     * use default alignment.
     */
    virtual void addLeftBarWidget(QWidget *widget,
                                  int stretch = 0,
                                  Qt::Alignment alignment = 0) = 0;

    /*!
     * \brief Add spacing to left bar. This is used for make some spacing
     * between the widgets.
     * \param size The spacing size.
     */
    virtual void addLeftBarSpacing(int size) = 0;

signals:

public slots:

private:
};

#endif // KNMAINWINDOWLEFTBARBASE_H
