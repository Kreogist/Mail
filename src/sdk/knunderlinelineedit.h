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
#ifndef KNUNDERLINELINEEDIT_H
#define KNUNDERLINELINEEDIT_H

#include <QLineEdit>

class QTimeLine;
/*!
 * \brief The KNUnderLineLineEdit class provides a special line edit widget.
 * This widget didn't change the signal or operation of the original line edit,
 * it only modified for the user interface.
 */
class KNUnderLineLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNUnderLineLineEdit widget with parent widget pointer.
     * \param parent The parent widget pointer.
     */
    explicit KNUnderLineLineEdit(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Update the line edit widget palette. Do not use setPalette() to
     * change the palette.
     * \param pal The new palette of the widget.
     */
    void updatePalette(const QPalette &pal);

protected:
    /*!
     * \brief Reimplemented from QLineEdit::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLineEdit::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLineEdit::focusInEvent().
     */
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLineEdit::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLineEdit::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionUpdateOpacity(int opacity);

private:
    inline void startAnime(int targetOpacity);
    QTimeLine *m_timeLine;
    int m_currentOpacity;
};

#endif // KNUNDERLINELINEEDIT_H
