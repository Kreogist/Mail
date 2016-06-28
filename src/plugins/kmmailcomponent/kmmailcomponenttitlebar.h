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

#include <QDateTime>

#include <QWidget>

class QLabel;
class QBoxLayout;
class KMMailContactButton;
class KMMailComponentContactArea;
class KMMailComponentContactList;
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

    /*!
     * \brief Whether from or to list is expaned.
     * \return If any of the contact list is expanded, this will return true.
     */
    bool isExpand() const;

    /*!
     * \brief reset
     */
    void reset();

signals:
    /*!
     * \brief When any size is changed, this signal will be emitted.
     */
    void titleSizeUpdate();

public slots:
    /*!
     * \brief Set the receiver user list.
     * \param addressList The address list.
     */
    void setReceiverList(const QStringList &addressList);

    /*!
     * \brief Set the sender list.
     * \param senderList The address list.
     */
    void setSenderList(const QStringList &senderList);

    /*!
     * \brief Set the title of the mail.
     * \param text The mail title text.
     */
    void setTitle(const QString &text);

    /*!
     * \brief Set the receive date of the mail.
     * \param receiveDate The receive date of the mail.
     */
    void setReceiveDate(const QDateTime &receiveDate);

    /*!
     * \brief updateHeight
     * \param targetWidth
     */
    void updateHeight(int targetWidth=-1);

private slots:
    void retranslate();
    void onThemeChanged();
    void onActionExpandChanged();

private:
    inline KMMailContactButton *generateButton(const QString &address);
    QList<KMMailContactButton *> m_fromList, m_toList;
    QString m_noTitle;
    QDateTime m_receiveDate;
    QBoxLayout *m_mainLayout;
    QLabel *m_titleLabel, *m_receiveLabel, *m_fromLabel, *m_toLabel;
    KMMailComponentContactArea *m_fromArea, *m_toArea;
    KMMailComponentContactList *m_fromListWidget, *m_toListWidget;
};

#endif // KMMAILCOMPONENTTITLEBAR_H
