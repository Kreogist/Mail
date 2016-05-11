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

#ifndef KMMAINWINDOW_H
#define KMMAINWINDOW_H

#include <QMainWindow>

class QTimeLine;
class KMCoverLayer;
class KNConfigure;
class KMMainWindowContainer;
class KMTitleBarBase;
class KMUnibarBase;
/*!
 * \brief The KMMainWindow class provides the main window of the Mail
 * application. All the plugins should be add to main window.
 */
class KMMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMainWindow widget with given parent.
     * \param parent The parent widget.
     */
    explicit KMMainWindow(QWidget *parent = 0);

signals:

public slots:
    /*!
     * \brief Set the title bar widget.
     * \param titleBar The title bar widget pointer.
     */
    void setTitleBar(KMTitleBarBase *titleBar);

    /*!
     * \brief Set the mail list widget.
     * \param mailList The mail list widget pointer.
     */
    void setMailList(QWidget *mailList);

    /*!
     * \brief Set the unibar widget.
     * \param uniBar The unibar widget pointer.
     */
    void setUniBar(KMUnibarBase *uniBar);

    /*!
     * \brief Set the mail component widget.
     * \param mailComponent Mail component widget pointer.
     */
    void setMailComponent(QWidget *mailComponent);

    /*!
     * \brief Set the preference widget.
     * \param preference The preference widget pointer.
     */
    void setPreference(QWidget *preference);

protected:
    /*!
     * \brief Reimplemented from QMainWindow::closeEvent().
     */
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QMainWindow::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionFullScreen();
    void onActionShowHideFloatLayer(int frame);
    void showUnibar();
    void hideUnibar();
    void showPreference();
    void hidePreference();
    void onActionHideFloatLayerFinished();
    void onActionHidePreferenceFinished();
    void onActionSwitchModel();

private:
    inline void startAnime(int endFrame);
    inline void recoverGeometry();
    inline void backupGeometry();
    inline int getCacheValue(const QString &valueName);
    inline void setCacheValue(const QString &valueName, int value);
    inline void zoomParameter(int &parameter, const qreal &ratio);
    Qt::WindowStates m_originalWindowState;
    KMMainWindowContainer *m_container;
    KMCoverLayer *m_floatLayer;
    QTimeLine *m_floatAnime;
    KMTitleBarBase *m_titleBar;
    KMUnibarBase *m_uniBar;
    KNConfigure *m_cacheConfigure;
};

#endif // KMMAINWINDOW_H
