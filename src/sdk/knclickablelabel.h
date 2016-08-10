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
#ifndef KNCLICKABLELABEL_H
#define KNCLICKABLELABEL_H

#include <QLabel>

/*!
 * \brief The KNClickableLabel class provides a widget which would detect mouse
 * click event.
 */
class KNClickableLabel : public QLabel
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNClickableLabel widget.
     * \param parent The parent widget.
     */
    explicit KNClickableLabel(QWidget *parent = 0);

signals:
    /*!
     * \brief When user clicked this label, this signal will be emit.
     */
    void clicked();

public slots:

protected:
    /*!
     * \brief Reimplemented from QLabel::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLabel::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_isPressed;
};

#endif // KNCLICKABLELABEL_H
