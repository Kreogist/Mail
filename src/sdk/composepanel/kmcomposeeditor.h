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
#ifndef KMCOMPOSEEDITOR_H
#define KMCOMPOSEEDITOR_H

#include <QWidget>

class QFontComboBox;
class QComboBox;
class KMComposeTextEdit;
class KMComposeButton;
/*!
 * \brief The KMComposeEditor class provides a fully rich text editor and it
 * could provides the html source code from the rich text.
 */
class KMComposeEditor : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMComposeEditor widget.
     * \param parent The parent widget.
     */
    explicit KMComposeEditor(QWidget *parent = 0);

signals:

public slots:

private slots:
    void onCursorPositionChange();
    void onActionChangeTheme();

    void onActionChangeBold(bool has);
    void onActionChangeItalic(bool has);
    void onActionChangeUnderline(bool has);
    void onActionChangeFont(const QString &fontFamily);
    void onActionChangeFontSize(const QString &fontSize);

private:
    inline void initialTools();
    inline void generateButton(int index, const QString &iconPath);
    inline void updateFormatState(int index, bool has);
    enum TextEditTools
    {
        ToolBold,
        ToolItalic,
        ToolUnderline,
        ToolIncreaseIndent,
        ToolDecreaseIndent,
        TextEditToolCount
    };
    KMComposeButton *m_toolButton[TextEditToolCount];
    QFontComboBox *m_fontFamily;
    QComboBox *m_fontSize;
    KMComposeTextEdit *m_editArea;
};

#endif // KMCOMPOSEEDITOR_H
