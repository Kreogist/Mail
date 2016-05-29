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
#ifndef KMUNIBARBUTTON_H
#define KMUNIBARBUTTON_H

#include <QAbstractButton>

class KMMailListModel;
class KMUnibarButton : public QAbstractButton
{
    Q_OBJECT
public:
    explicit KMUnibarButton(QWidget *parent = 0);

    qreal opacity() const;

signals:

public slots:
    void setOpacity(const qreal &opacity);
    void setMailListModel(KMMailListModel *model);

protected:
    /*!
     * \brief paintEvent
     * \param event
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief enterEvent
     * \param event
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:

private:
    qreal m_opacity, m_hoverOpacity;
};

#endif // KMUNIBARBUTTON_H
