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
#ifndef KMMAILCOMPONENTTITLEBAR_H
#define KMMAILCOMPONENTTITLEBAR_H

#include <QDate>

#include <QWidget>

class QLabel;
/*!
 * \brief The KMMailComponentTitleBar class provides a title bar which could
 * display the basic information about the mail itself.
 */
class KMMailComponentTitleBar : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailComponentTitleBar widget.
     * \param parent The parent widget.
     */
    explicit KMMailComponentTitleBar(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the title of the mail.
     * \param text The mail title text.
     */
    void setTitle(const QString &text);

    /*!
     * \brief Set the receive date of the mail.
     * \param receiveDate The receive date of the mail.
     */
    void setReceiveDate(const QDate &receiveDate);

private slots:
    void retranslate();

private:
    QDate m_receiveDate;
    QLabel *m_titleLabel, *m_receiveLabel, *m_fromLabel, *m_toLabel;
};

#endif // KMMAILCOMPONENTTITLEBAR_H
