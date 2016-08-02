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
Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMAILCONTACTFLOWLAYOUT_H
#define KNMAILCONTACTFLOWLAYOUT_H

#include <QStyle>
#include <QLayout>

/*!
 * \brief The KNMailContactFlowLayout class is a flow layout modified from the
 * official flow layout. The original flow layout cannot just fit our
 * requirement so that we modified it for the contact widget.
 */
class KNMailContactFlowLayout : public QLayout
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailContactFlowLayout layout object to the hosted
     * widget with parameters.
     * \param hSpacing The vertical spacing of the widgets.
     * \param vSpacing The horizontal spacing of the widgets.
     * \param parent The parent widget pointer.
     */
    explicit KNMailContactFlowLayout(int hSpacing = -1,
                                     int vSpacing = -1,
                                     QWidget *parent = 0);
    ~KNMailContactFlowLayout();

    /*!
     * \brief Reimplemented from QLayout::addItem().
     */
    void addItem(QLayoutItem *item) Q_DECL_OVERRIDE;

    /*!
     * \brief Get the horizontal layout spacing.
     * \return The horizontal spacing value.
     */
    int horizontalSpacing() const;

    /*!
     * \brief Get the vertical layout spacing.
     * \return The vertical widget spacing of the layout.
     */
    int verticalSpacing() const;

    /*!
     * \brief Reimplemented from QLayout::expandingDirections().
     */
    Qt::Orientations expandingDirections() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::hasHeightForWidth().
     */
    bool hasHeightForWidth() const Q_DECL_OVERRIDE
    {
        return true;
    }

    /*!
     * \brief Reimplemented from QLayout::heightForWidth().
     */
    int heightForWidth(int width) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::setGeometry().
     */
    void setGeometry(const QRect &rect) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::sizeHint().
     */
    QSize sizeHint() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::minimumSize().
     */
    QSize minimumSize() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::itemAt().
     */
    QLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::takeAt().
     */
    QLayoutItem *takeAt(int index) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QLayout::count().
     */
    int count() const Q_DECL_OVERRIDE;

signals:

public slots:

private:
    inline int smartSpacing(QStyle::PixelMetric pm) const;
    inline int doLayout(QRect effectiveRect, bool apply=false) const;
    QList<QLayoutItem *> m_itemList;
    int m_hSpace, m_vSpace;
};

#endif // KNMAILCONTACTFLOWLAYOUT_H
