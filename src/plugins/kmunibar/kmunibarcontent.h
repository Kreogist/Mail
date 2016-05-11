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
#ifndef KMUNIBARCONTENT_H
#define KMUNIBARCONTENT_H

#include "mailaccount/kmmailaccount.h"

#include <QWidget>

class QBoxLayout;
class KMUnibarButton;
class KMUnibarAccountList;
/*!
 * \brief The KMUnibarContent class
 */
class KMUnibarContent : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMUnibarContent widget.
     * \param parent The parent widget.
     */
    explicit KMUnibarContent(QWidget *parent = 0);

signals:
    /*!
     * \brief switchModel
     * \param account
     * \param modelIndex
     */
    void switchModel(KMMailAccount *account, int modelIndex);

public slots:
    /*!
     * \brief addAccountList
     * \param accountList
     */
    void addAccountList(KMUnibarAccountList *accountList);

private slots:
    void onActionChangeSize(int heightDelta);
    void onActionChangeModel(int modelIndex);

private:
    QBoxLayout *m_mainLayout;
};

#endif // KMUNIBARCONTENT_H
