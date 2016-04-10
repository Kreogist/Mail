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
#ifndef KMMAILLISTVIEW_H
#define KMMAILLISTVIEW_H

#include <QListView>

class QTimeLine;
class QScrollBar;
/*!
 * \brief The KMMailListView class provides a list view to display the mail
 * list.
 */
class KMMailListView : public QListView
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailListView widget.
     * \param parent The parent widget pointer.
     */
    explicit KMMailListView(QWidget *parent = 0);

    /*!
     * \brief Update the object name set up the new palette from theme manager.
     * \param name The new object name.
     */
    void updateObjectName(const QString &name);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from QListView::enterEvent().
     */
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::leaveEvent().
     */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QListView::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

protected slots:
    /*!
     * \brief This slot is provide to update the palette when the tree view is
     * being constructed. Or else the UI will be very ugly.
     */
    void onActionThemeUpdate();

private slots:
    void onActionMouseInOut(int frame);

private:
    inline void startAnime(int endFrame);
    QTimeLine *m_mouseAnime;
    QScrollBar *m_scrollBar;
};

#endif // KMMAILLISTVIEW_H
