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

#ifndef KNMAILPOPUPMANAGER_H
#define KNMAILPOPUPMANAGER_H

#include <QHash>

#include <QObject>

/*!
 * \def knMailPopupManager
 * A global pointer referring to the unique mail popup window manager object.
 */
#define knMailPopupManager (KNMailPopupManager::instance())

class KNMailViewerBase;
/*!
 * \brief The KNMailPopupManager class will manage all the popup window. When
 * the user is asking for popup an E-mail which is already poped up, then .
 */
class KNMailPopupManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Get the global instance of the mail popup window manager.
     * \return The mail viewer window manager instance.
     */
    static KNMailPopupManager *instance();

    /*!
     * \brief Initial the global instance pointer with the given parent.
     * \param parent The parent object.
     */
    static void initial(QObject *parent);

    /*!
     * \brief Get the mail viewer window pointer.
     * \param filePath The file path.
     * \return Get the viewer window pointer. If there's no window match the
     * file path, it will return a nullptr.
     */
    KNMailViewerBase *window(const QString &filePath);

signals:

public slots:
    /*!
     * \brief Append a popup window to the manager.
     * \param filePath The file path that this window shows.
     * \param window The window widget.
     */
    void appendWindow(const QString &filePath, KNMailViewerBase *window);

    /*!
     * \brief Show the popup window according the mail file path.
     * \param filePath The mail file path.
     */
    void popupMail(const QString &filePath);

    /*!
     * \brief Close all mail viewer.
     */
    void closeAllMail();

private slots:
    void removeWindow(const QString &filePath);

private:
    static KNMailPopupManager *m_instance;
    explicit KNMailPopupManager(QObject *parent = 0);
    KNMailPopupManager(const KNMailPopupManager &);
    KNMailPopupManager(KNMailPopupManager &&);
    QHash<QString, KNMailViewerBase *> m_pathWindowList;
};

#endif // KNMAILPOPUPMANAGER_H
