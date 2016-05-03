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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KMMAILCOMPONENTCONTACTLIST_H
#define KMMAILCOMPONENTCONTACTLIST_H

#include <QWidget>

class QBoxLayout;
class KMExtendButton;
/*!
 * \brief The KMMailComponetContactList class provides a list for all the
 * contact items. It will automatically update the size when the list is expand
 * or fold.
 */
class KMMailComponentContactList : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailComponetContactList widget with given parent
     * widget.
     * \param parent The parent widget pointer.
     */
    explicit KMMailComponentContactList(QWidget *parent = 0);

    /*!
     * \brief Add a widget to the contact list.
     * \param widget The widget pointer.
     */
    void addWidget(QWidget *widget);

    /*!
     * \brief Clear the list.
     */
    void clear();

    /*!
     * \brief When the expand is enabled, return this list is expanded or not.
     * \return Whether the list is expanded.
     */
    bool isExpaned() const;

signals:
    /*!
     * \brief When expand state is changed, this signal will be emitted.
     */
    void expandStateChange();

public slots:
    /*!
     * \brief Set whether folding the list is enabled.
     * \param enableFold To enable folding, set this value to true.
     */
    void setEnableFold(bool enableFold);

private slots:
    void onActionChangeExpand();

private:
    inline void updateLayout();
    inline void updateExpandedSize();
    QList<QWidget *> m_buttonList;
    QBoxLayout *m_listLayout;
    KMExtendButton *m_extendButton;
    bool m_fold, m_isFold;
};

#endif // KMMAILCOMPONENTCONTACTLIST_H
