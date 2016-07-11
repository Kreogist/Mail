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
#ifndef KNMAILFOLDERVIEWERTITLE_H
#define KNMAILFOLDERVIEWERTITLE_H

#include <QWidget>

/*!
 * \brief The KNMailFolderViewerTitle class provides the title of the folder, it
 * will display the folder name.
 */
class KNMailFolderViewerTitle : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailFolderViewerTitle widget.
     * \param parent The parent widget.
     */
    explicit KNMailFolderViewerTitle(QWidget *parent = 0);

    /*!
     * \brief Get the title display folder name.
     * \return The folder name. Default is empty string.
     */
    QString folderName() const;

signals:

public slots:
    /*!
     * \brief Set the title folder name.
     * \param folderName The folder name which will be displayed.
     */
    void setFolderName(const QString &folderName);

    /*!
     * \brief Clear the title bar folder name text.
     */
    void clear();

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_folderName;
};

#endif // KNMAILFOLDERVIEWERTITLE_H
