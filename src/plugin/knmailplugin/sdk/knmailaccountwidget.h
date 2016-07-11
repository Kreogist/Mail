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
#ifndef KNMAILACCOUNTWIDGET_H
#define KNMAILACCOUNTWIDGET_H

#include <QWidget>

#define ItemHeight 24
#define ItemSpacing 7
#define ItemMargin 3
#define ItemShadowHeight 10

class QTimeLine;
class KNMailModel;
class KNMailAccount;
class KNMailAccountButton;
/*!
 * \brief The KNMailAccountWidget class provides a view of mail account. It will
 * emit the model change signal for switching the model.\n
 * When expanding an account widget, it will asking for switching the model into
 * the inbox of the account. When folding an account widget, it will reset all
 * the selection of the current managed account.
 */
class KNMailAccountWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailAccountWidget widget with given account.
     * \param account The managed mail account.
     * \param parent The parent widget.
     */
    explicit KNMailAccountWidget(KNMailAccount *account, QWidget *parent = 0);

    /*!
     * \brief Set the account global shadow gradient.
     * \param shadowGradient The shadow gradient.
     */
    static void setShadowGradient(const QLinearGradient &shadowGradient);

    /*!
     * \brief Get the current model.
     * \return The current model.
     */
    KNMailModel *currentModel();

    /*!
     * \brief Get the expanded height of the current account widget.
     * \return The expanded height of the widget.
     */
    int expandedHeight();

signals:
    /*!
     * \brief When the mail account expanded, this signal will be emitted.
     * \param targetHeight The target height of the panel.
     */
    void panelExpanded(int targetHeight);

    /*!
     * \brief When the mail panel folded, this signal will be folded.
     */
    void panelFolded();

    /*!
     * \brief When user click one folder, this signal will be emitted for
     * requiring to show the folder.
     * \param folder The folder model.
     */
    void requireShowFolder(KNMailModel *folder);

public slots:
    /*!
     * \brief Set whether the panel is expanded.
     * \param expand Expanded state.
     */
    void setExpand(bool expand);

    /*!
     * \brief Fold the account information panel.
     */
    void foldPanel();

    /*!
     * \brief Expand the account information panel.
     */
    void expandPanel();

    /*!
     * \brief Set the selected model index. To remove the selected folder, set
     * the index to be -1.
     * \param index The folder index.
     */
    void setCurrentIndex(int index);

protected:
    /*!
     * \brief Reimplemented from QWidget::mousePressEvent().
     */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::mouseReleaseEvent().
     */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionResizePanel(int currentHeight);

private:
    enum MailAccountButtons
    {
        ButtonSync,
        ButtonOption,
        ButtonExpand,
        MailAccountButtonCount
    };
    inline void updateExpandParameters();
    inline void updateFoldParameters();
    inline void startHeightAnime(int targetHeight);
    static QLinearGradient m_shadowGradient;
    KNMailAccountButton *m_button[MailAccountButtonCount];
    qreal m_animeProgress;
    QTimeLine *m_expandAnime;
    KNMailAccount *m_account;
    int m_selectedIndex;
    bool m_expanded, m_drawContent, m_isPressed;
};

#endif // KNMAILACCOUNTWIDGET_H
