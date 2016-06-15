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
#ifndef KMLEFTBAR_H
#define KMLEFTBAR_H

#include "kmleftbarbase.h"

class KNSideShadowWidget;
class KMMailListView;
/*!
 * \brief The KMLeftBar class provides an official release version of the mail
 * application left bar.
 */
class KMLeftBar : public KMLeftBarBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMLeftBar widget.
     * \param parent The parent widget.
     */
    explicit KMLeftBar(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Reimplemented from KMLeftBarBase::setMailListModel().
     */
    void setMailListModel(KMMailListModel *model) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KMLeftBarBase::switchModel().
     */
    void switchModel(KMMailAccount *account, int modelIndex) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KMLeftBarBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    KMMailListView *m_mailList;
    KNSideShadowWidget *m_topShadow;
};

#endif // KMLEFTBAR_H
