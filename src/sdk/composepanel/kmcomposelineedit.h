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
#ifndef KMCOMPOSELINEEDIT_H
#define KMCOMPOSELINEEDIT_H

#include <QLineEdit>

class QTimeLine;
/*!
 * \brief The KMComposeLineEdit class provides a widget which could be display
 * the content of the current edit data for the compose panel.\n
 */
class KMComposeLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMComposeLineEdit widget.
     * \param parent The parent widget.
     */
    explicit KMComposeLineEdit(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QLineEdit::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLineEdit::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLineEdit::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_isEntered;
};

#endif // KMCOMPOSELINEEDIT_H
