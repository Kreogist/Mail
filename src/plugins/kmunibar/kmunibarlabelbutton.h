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
#ifndef KMUNIBARLABELBUTTON_H
#define KMUNIBARLABELBUTTON_H

#include <QWidget>

/*!
 * \brief The KMUnibarLabelButton class
 */
class KMUnibarLabelButton : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KMUnibarLabelButton widget.
     * \param parent The parent widget.
     */
    explicit KMUnibarLabelButton(QWidget *parent = 0);

    /*!
     * \brief Get current rotate
     */
    int rotate() const;

    QPixmap image() const;

signals:
    /*!
     * \brief Label button clicked
     */
    void clicked();

public slots:
    /*!
     * \brief setImage
     * \param image
     */
    void setImage(const QPixmap &image);

    /*!
     * \brief setRotate
     * \param rotate
     */
    void setRotate(int rotate);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPixmap m_image;
    int m_rotate;
};

#endif // KMUNIBARLABELBUTTON_H
