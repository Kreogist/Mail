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
#ifndef KNMAILACCOUNTLIST_H
#define KNMAILACCOUNTLIST_H

#include <QHash>

#include "knmailmodel.h"

#include "knsensescrollarea.h"

class QBoxLayout;
class QTimeLine;
class KNMailAccount;
class KNMailAccountWidget;
/*!
 * \brief The KNMailAccountList class provides a widget which could hold and
 * manage all the account as a widget. The widget will be dynamicly add and
 * removed.
 */
class KNMailAccountList : public KNSenseScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailAccountList widget.
     * \param parent The parent widget.
     */
    explicit KNMailAccountList(QWidget *parent = 0);

    /*!
     * \brief Get the current model.
     * \return The current model.
     */
    KNMailModel *currentModel();

signals:
    /*!
     * \brief When user click one folder, this signal will be emitted for
     * requiring to show the folder.
     * \param folder The folder model.
     */
    void requireShowFolder(KNMailModel *folder);

public slots:
    /*!
     * \brief Add one account widget to the end of the list.
     * \param accountWidget The account widget list item.
     */
    void addAccountWidget(KNMailAccountWidget *accountWidget);

protected:
    /*!
     * \brief Reimplemented from KNSenseScrollArea::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNSenseScrollArea::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNSenseScrollArea::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionThemeChanged();
    void onActionAccountAdded(int accountIndex);
    void onActionPanelExpanded(int panelHeight);
    void onActionPanelFold();
    void onActionMouseInOut(int opacity);
    void onActionShowFolder(KNMailModel *folder);
    void onPanelSizeChange(int previousSize, int panelSize);

private:
    inline void startScrollAnime(int targetAlpha);
    QList<KNMailAccountWidget *> m_accountList;
    QHash<QObject *, int> m_heightDeltaList;
    QPalette m_accountPalette;
    QTimeLine *m_expandAnime, *m_mouseAnime;
    QWidget *m_container;
    QBoxLayout *m_containerLayout;
    int m_containerHeight, m_currentIndex;
};

#endif // KNMAILACCOUNTLIST_H
