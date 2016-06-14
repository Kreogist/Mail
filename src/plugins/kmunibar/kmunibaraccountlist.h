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

    /*!
     * \brief Get the account label text. It should be the account name with the
     * addcount address.
     * \return The account label text.
     */
    QString labelText() const;

    /*!
     * \brief Get the specific foldertext.
     * \param modelIndex The index of specific folder
     * \return The name of folder.
     */
    QString folderText(int modelIndex) const;

    /*!
     * \brief Get the account object currently managed.
     */
    KMMailAccount *account() const;

    /*!
     * \brief Set a new mail account.
     * \param account The account object, default is a NULL.
     */
    void setAccount(KMMailAccount *account);

    /*!
     * \brief reset the size and update.
     */
    void reset();

signals:
    /*!
     * \brief sizeChanged
     * \param heightDelta
     */
    void sizeChanged(int heightDelta);

    /*!
     * \brief currentModelChanged
     * \param modelIndex
     */
    void currentModelChanged(int modelIndex);

public slots:
    /*!
     * \brief Set the wiget text.
     * \param text.
     */
    void setText(const QString &labelText);

protected:
    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onThemeChanged();
    void onActionExpandWidget(int widgetHeight);
    void onActionChangeExpand();
    void onActionShowFinished();
    void onActionHideFinished();
    void onActionButtonClicked();
    void onActionAccountPropertyChange();

private:
    inline void addFolder(KMUnibarButton *button);
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
