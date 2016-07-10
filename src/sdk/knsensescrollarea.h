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
#ifndef KNSENSESCROLLAREA_H
#define KNSENSESCROLLAREA_H

#include <QScrollArea>

#define ScrollBarWidth (10)

/*!
 * \brief The KNSenseScrollArea class provides a special scroll area. It didn't
 * change nearly anything as the default scroll area expect the vertical scroll
 * bar. If you are using this scroll area, don't chagne the settings of the
 * vertical scroll bar.\n
 * It doesn't write the animation for the scroll bar, it also explode the scroll
 * bar pointer for further customized.
 */
class KNSenseScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNSenseScrollArea widget with given parent widget.
     * \param parent The parent widget.
     */
    explicit KNSenseScrollArea(QWidget *parent = 0);


signals:

public slots:

protected:
    /*!
     * \brief Get the custom scroll bar widget pointer for further customized.
     * \return The widget pointer.
     */
    QScrollBar *customScrollBar();

    /*!
     * \brief Reimplemented from QScrollArea::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionRangeChanged(int min, int max);
    void onActionValueChanged(int value);

private:
    inline void updateScrollBarGeometry();
    QScrollBar *m_scrollBar;
};

#endif // KNSENSESCROLLAREA_H
