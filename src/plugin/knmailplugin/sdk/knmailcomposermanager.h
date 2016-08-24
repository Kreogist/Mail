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

#ifndef KNMAILCOMPOSERMANAGER_H
#define KNMAILCOMPOSERMANAGER_H

#include <QList>

#include <QObject>

/*!
 * \def knMailComposerManager
 * A global pointer referring to the unique mail composer manager instance.
 */
#define knMailComposerManager (KNMailComposerManager::instance())

class KNMailComposeBase;
/*!
 * \brief The KNMailComposerManager class provides the composer widget manager.
 * When it generate a new widget, it will add to composer manager.
 */
class KNMailComposerManager : public QObject
{
    Q_OBJECT
public:
    ~KNMailComposerManager();

    /*!
     * \brief Get the composer manager singleton instance pointer.
     * \return The instance pointer.
     */
    static KNMailComposerManager *instance();

    /*!
     * \brief Construct the global instance.
     * \param parent The parent object.
     */
    static void initial(QObject *parent);

    /*!
     * \brief Append the composer to the manager.
     * \param composer The widget pointer.
     */
    void append(KNMailComposeBase *composer);

signals:

public slots:

private slots:
    void onComposeWindowClose();

private:
    static KNMailComposerManager *m_instance;
    explicit KNMailComposerManager(QObject *parent = 0);
    KNMailComposerManager(const KNMailComposerManager &);
    KNMailComposerManager(KNMailComposerManager &&);
    QList<KNMailComposeBase *> m_composerList;
};

#endif // KNMAILCOMPOSERMANAGER_H
