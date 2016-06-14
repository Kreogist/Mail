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

#ifndef KMMAILCOMPONENTCONTENTBASE_H
#define KMMAILCOMPONENTCONTENTBASE_H

#include <QWidget>

class KNSideShadowWidget;
/*!
 * \brief The KMMailComponentContentBase class provides a widget to show the
 * content of the mail content.\n
 * This is a base widget. It means that it should be implemented and realize the
 * the new widget.
 */
class KMMailComponentContentBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailComponentContentBase widget.
     * \param parent The parent widget.
     */
    explicit KMMailComponentContentBase(QWidget *parent = 0);

signals:

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    KNSideShadowWidget *m_topShadow;
};

#endif // KMMAILCOMPONENTCONTENTBASE_H
