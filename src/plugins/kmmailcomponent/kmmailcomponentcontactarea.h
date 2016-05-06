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
#ifndef KMMAILCOMPONENTCONTACTAREA_H
#define KMMAILCOMPONENTCONTACTAREA_H

#include <QScrollArea>

class QTimeLine;
class KNSideShadowWidget;
class KMMailComponentContactList;
/*!
 * \brief The KMMailComponentContactArea class
 */
class KMMailComponentContactArea : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief KMMailComponentContactArea
     * \param parent
     */
    explicit KMMailComponentContactArea(QWidget *parent = 0);

signals:
    /*!
     * \brief When contact area size is changed, this signal will be emitted.
     */
    void areaHeightChange();

public slots:
    /*!
     * \brief setContactList
     * \param list
     */
    void setContactList(KMMailComponentContactList *list);

protected:
    /*!
     * \brief Reimplemented from QScrollArea::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QScrollArea::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QScrollArea::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionMouseInOut(int frame);
    void onActionRangeChange(int min, int max);
    void onExpandStateChange();
    void updateGeometries();

private:
    inline void startAnime(int endFrame);
    inline void updateShadow(int value);
    QTimeLine *m_mouseAnime;
    QScrollBar *m_scrollBar;
    KMMailComponentContactList *m_contactList;
    KNSideShadowWidget *m_topShadow, *m_bottomShadow;
};

#endif // KMMAILCOMPONENTCONTACTAREA_H
