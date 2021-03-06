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

#ifndef KNMAILCOMPOSE_H
#define KNMAILCOMPOSE_H

#include "knmailcomposebase.h"

class QPushButton;
class QComboBox;
class QLabel;
class QBoxLayout;
class KNMailComposeEdit;
class KNUnderLineLineEdit;
/*!
 * \brief The KNMailCompose class provides the official composing panel. It will
 * use all the SDKs to build the composing window.
 */
class KNMailCompose : public KNMailComposeBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailCompose widget.
     * \param parent The parent widget pointer.
     */
    explicit KNMailCompose(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMailComposeBase::closeEvent().
     */
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onThemeChanged();
    void onSendClicked();
    void onSenderListSizeChange();

private:
    QBoxLayout *m_mainLayout;
    KNUnderLineLineEdit *m_subject;
    KNMailComposeEdit *m_textEditor;
    QLabel *m_senderLabel, *m_receiverLabel, *m_ccLabel, *m_bccLabel,
           *m_attachment;
    QComboBox *m_senderText;
    KNUnderLineLineEdit *m_receiverText, *m_ccText, *m_bccText;
    QPushButton *m_sendMail;
};

#endif // KNMAILCOMPOSE_H
