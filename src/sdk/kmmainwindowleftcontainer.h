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
#ifndef KMMAINWINDOWLEFTCONTAINER_H
#define KMMAINWINDOWLEFTCONTAINER_H

#include <QWidget>

class QBoxLayout;
/*!
 * \brief The KMMainWindowLeftContainer class provides the container of all the
 * left part widgets.
 */
class KMMainWindowLeftContainer : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KMMainWindowLeftContainer widget.
     * \param parent The parent widget.
     */
    explicit KMMainWindowLeftContainer(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the title bar widget.
     * \param titleBar The title bar widget pointer.
     */
    void setTitleBar(QWidget *titleBar);

    /*!
     * \brief Set the mail list widget.
     * \param mailList The mail list widget pointer.
     */
    void setMailList(QWidget *mailList);

    /*!
     * \brief Set the unibar widget.
     * \param uniBar The unibar widget pointer.
     */
    void setUniBar(QWidget *uniBar);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget *m_titleBar, *m_mailList, *m_uniBar;
};

#endif // KMMAINWINDOWLEFTCONTAINER_H
