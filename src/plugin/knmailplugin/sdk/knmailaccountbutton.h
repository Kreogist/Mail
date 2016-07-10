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
#ifndef KNMAILACCOUNTBUTTON_H
#define KNMAILACCOUNTBUTTON_H

#include "knopacitybutton.h"

/*!
 * \brief The KNMailAccountButton class provides the button widget of the mail
 * account title bar. It will provides the button function as the normal opacity
 * animation button, but also provide the rotate function.
 */
class KNMailAccountButton : public KNOpacityButton
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailAccountButton widget with given parent widget.
     * \param parent The parent widget pointer.
     */
    explicit KNMailAccountButton(QWidget *parent = 0);

    /*!
     * \brief Get the current rotate of the button.
     * \return The image rotate degree. Default is 0.
     */
    int rotate() const;

signals:

public slots:
    /*!
     * \brief Set image rotate degree.
     * \param rotate The image rotate degree.
     */
    void setRotate(int rotate);

protected:
    /*!
     * \brief Reimplemented from KNOpacityButton::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int m_rotate;
};

#endif // KNMAILACCOUNTBUTTON_H
