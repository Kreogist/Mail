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
#ifndef KMFLOWLAYOUT_H
#define KMFLOWLAYOUT_H

#include <QRect>
#include <QStyle>

#include <QLayout>

/*!
 * \brief The KMFlowLayout implements a layout that handles different window
 * sizes. The widget placement changes depending on the width of the application
 * window.\n
 * Most of the codes is from Qt's example. We optimized a little for our own
 * codes.
 */
class KMFlowLayout : public QLayout
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMFlowLayout layout.
     * \param margin The margin of the layout. Default is -1.
     * \param hSpacing The horizontal spacing. Default is -1.
     * \param vSpacing The vertical spacing. Default is -1.
     * \param parent The parent widget pointer.
     */
    explicit KMFlowLayout(int margin = -1,
                          int hSpacing = -1,
                          int vSpacing = -1,
                          QWidget *parent = 0);
    ~KMFlowLayout();

    /*!
     * \brief Reimplemented from QLayout::addItem().
     */
    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the horizontal spacing.
     * \return Horizontal spacing. Default is depends on system layout.
     */
    int horizontalSpacing() const;

    /*!
     * \brief Get the vertical spacing.
     * \return Vertical spacing. Default is depends on system layout.
     */
    int verticalSpacing() const;

    /*!
     * \brief Reimplemented from QLayout::expandingDirections().
     */
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::hasHeightForWidth().
     */
    bool hasHeightForWidth() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::heightForWidth().
     */
    int heightForWidth(int) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::count().
     */
    int count() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::itemAt().
     */
    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::minimumSize().
     */
    QSize minimumSize() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::setGeometry().
     */
    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::takeAt().
     */
    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    inline int doLayout(const QRect &rect, bool testOnly) const;
    inline int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList;
    int m_hSpace;
    int m_vSpace;
};

#endif // KMFLOWLAYOUT_H
