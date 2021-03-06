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

#ifndef KNMAILACCOUNTADDDIALOG_H
#define KNMAILACCOUNTADDDIALOG_H

#include <QSsl>
#include "knmailutil.h"

#include "knmessagebox.h"

using namespace MailUtil;

class QLabel;
class KNHWidgetSwitcher;
/*!
 * \brief The KNMailAccountAddDialog class provides the add dialog of the
 */
class KNMailAccountAddDialog : public KNMessageBox
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailAccountAddDialog dialog.
     * \param parent The parent widget.
     */
    explicit KNMailAccountAddDialog(QWidget *parent = 0);

signals:

public slots:

protected:
    /*!
     * \brief Reimplemented from KNMessageBox::showEvent().
     */
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    inline void getProtocolSetting(int index,
                                   MailProtocolSocket &protocol,
                                   QSsl::SslProtocol &type);
    enum AddAccountPanels
    {
        UsernamePasswordPanel,
        ConfigurationPanel,
        SuccessPanel,
        AddAccountPanelsCount
    };
    QWidget *m_panelContainer[AddAccountPanelsCount];
    QLabel *m_loginHintText;
    KNHWidgetSwitcher *m_widgetSwitcher;
};

#endif // KNMAILACCOUNTADDDIALOG_H
