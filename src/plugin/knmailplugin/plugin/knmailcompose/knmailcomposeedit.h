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

#ifndef KNMAILCOMPOSEEDIT_H
#define KNMAILCOMPOSEEDIT_H

#include <QTextEdit>

class QFontComboBox;
class QComboBox;
class QTimeLine;
class KNOpacityPressedButton;
/*!
 * \brief The KNMailComposeEdit class provides a widget which allows user to
 * edit an mail. It provides the function to translate the content to multipart
 * mime type.
 */
class KNMailComposeEdit : public QTextEdit
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailComposeEdit widget
     * \param parent The parent widget.
     */
    explicit KNMailComposeEdit(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QTextEdit::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QTextEdit::focusInEvent().
     */
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QTextEdit::focusOutEvent().
     */
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private slots:

private:
    enum ControlButtons
    {
        ButtonBold,
        ButtonItalic,
        ButtonUnderline,
        ControlButtonCount
    };

    inline void setBlockStatus(bool enabled);
    inline void startAnime(int endFrame);
    inline void updateToolBarGeometry();
    KNOpacityPressedButton *m_statusButton[ControlButtonCount];
    QWidget *m_toolBar;
    QFontComboBox *m_fontBox;
    QComboBox *m_fontSizeBox;
    QTimeLine *m_toolBarAnime;
    int m_shownHeight;
};

#endif // KNMAILCOMPOSEEDIT_H
