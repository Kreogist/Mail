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
#ifndef KMCOMPOSEPANEL_H
#define KMCOMPOSEPANEL_H

#include <QWidget>

class QLabel;
class QComboBox;
class KMComposeEditor;
class KMComposeLineEdit;
/*!
 * \brief The KMComposePanel class provides the compose panel which user could
 * compose E-mail in this widget.
 */
class KMComposePanel : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMComposePanel widget.
     * \param parent The parent widget.
     */
    explicit KMComposePanel(QWidget *parent = 0);

signals:

public slots:

private slots:
    void retranslate();

private:
    inline void updateTitle();
    QComboBox *m_from;
    KMComposeLineEdit *m_title, *m_receiver;
    QLabel *m_fromHint, *m_toHint;
    KMComposeEditor *m_editor;
    QString m_emptyText, m_titleText;
};

#endif // KMCOMPOSEPANEL_H
