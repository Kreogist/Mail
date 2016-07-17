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

#ifndef KNMOUSESENSEHEADER_H
#define KNMOUSESENSEHEADER_H

#include <QHeaderView>

class QTimeLine;
/*!
 * \brief The KNMouseSenseHeader class provides you a mouse sensitive tree view
 * header. It's a horizontal header widget. It doesn't provides any context
 * menu. So if you want to use it, please reimplement it and construct a context
 * menu.
 */
class KNMouseSenseHeader : public QHeaderView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMouseSenseHeader class.
     * \param parent The parent widget.
     */
    explicit KNMouseSenseHeader(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QHeaderView::enterEvent().
     */
    void enterEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QHeaderView::leaveEvent().
     */
    void leaveEvent(QEvent *event);

    /*!
     * \brief Reimplemented from QHeaderView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event);

    /*!
     * \brief Reimplemented from QHeaderView::paintSection().
     */
    void paintSection(QPainter *painter,
                      const QRect &rect,
                      int logicalIndex) const;

private slots:
    void onThemeChanged();
    void onActionMouseInOut(const int &frame);

private:
    inline void startAnime(const int &endFrame);
    QPixmap m_ascendingIcon, m_descendingIcon;
    QTimeLine *m_mouseAnime;
    int m_lineBrightness;
    QLinearGradient m_buttonGradient;
};

#endif // KNMOUSESENSEHEADER_H
