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
#ifndef KMUNIBARACCOUNTLIST_H
#define KMUNIBARACCOUNTLIST_H

#include <QWidget>

#include "kmmailutil.h"

using namespace MailUtil;

class QBoxLayout;
class QLabel;
class QTimeLine;
class KMMailAccount;
class KMUnibarButton;
class KMUnibarLabelButton;
class KMUnibarAccountList : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMUnibarAccountList widget.
     * \param parent The parent widget.
     */
    explicit KMUnibarAccountList(QWidget *parent = 0);

    QString accountLabel() const;

    KMMailAccount *currentAccount() const;
    void setCurrentAccount(KMMailAccount *currentAccount);

    void reset();

signals:
    void sizeChanged(int heightDelta);

    void currentModelChanged(int modelIndex);

public slots:
    void setAccountLabel(const QString &accountLabel);

protected:
    /*!
     * \brief paintEvent
     * \param event
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief resizeEvent
     * \param event
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onThemeChanged();
    void onActionExpandWidget(int widgetHeight);
    void onActionChangeExpand();
    void onActionShowFinished();
    void onActionHideFinished();
    void onActionButtonClicked();

private:
    inline void addToFolderList(KMUnibarButton *button);
    inline void startAnime(int endFrame);
    QString m_accountLabel;
    QList<KMUnibarButton *> m_folderList;
    KMUnibarButton *m_systemFolder[MailSystemFoldersCount];
    KMUnibarLabelButton *m_foldedButton;
    QTimeLine *m_animeTimeLine;
    KMMailAccount *m_currentAccount;
    int m_currentFolder;
    bool m_expand;
};

#endif // KMUNIBARACCOUNTLIST_H
