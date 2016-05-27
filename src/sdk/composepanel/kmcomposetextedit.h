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

#ifndef KMCOMPOSETEXTEDIT_H
#define KMCOMPOSETEXTEDIT_H

#include <QTextEdit>

class QTimeLine;
class QScrollBar;
/*!
 * \brief The KMComposeTextEdit class provides the text editor that fit the
 * style of compose window.
 */
class KMComposeTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    /*!
     * \brief KMComposeTextEdit
     * \param parent
     */
    explicit KMComposeTextEdit(QWidget *parent = 0);

    /*!
     * \brief Update the object name set up the new palette from theme manager.
     * \param name The new object name.
     */
    void updateObjectName(const QString &name);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QTextEdit::enterEvent()
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QTextEdit::leaveEvent()
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QTextEdit::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionThemeUpdate();
    void onActionMouseInOut(int frame);

private:
    inline void startAnime(int endFrame);
    QTimeLine *m_mouseAnime;
    QScrollBar *m_scrollBar;
};

#endif // KMCOMPOSETEXTEDIT_H
