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
#ifndef KMLEFTBARBASE_H
#define KMLEFTBARBASE_H

#include <QWidget>

class KMMailAccount;
class KMMailListModel;
/*!
 * \brief The KMLeftBarBase class provide the left bar port function of the mail
 * application left bar.
 */
class KMLeftBarBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMLeftBarBase widget.
     * \param parent The parent widget pointer.
     */
    KMLeftBarBase(QWidget *parent = 0): QWidget(parent){}

signals:

public slots:
    /*!
     * \brief switchModel
     * \param account
     * \param modelIndex
     */
    virtual void switchModel(KMMailAccount *account,
                             int modelIndex)=0;

    /*!
     * \brief Set the mail list model to the left bar, and display the model.
     * \param model The current model.
     */
    virtual void setMailListModel(KMMailListModel *model)=0;
};

#endif // KMLEFTBARBASE_H
