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
#ifndef KNMAINWINDOW_H
#define KNMAINWINDOW_H

#include <QMainWindow>

class QSplitter;
class KNMailPluginBase;
/*!
 * \brief The KNMainWindow class provides the main window of the Mail
 * application. It will host for all the visible widget. It might be hide but it
 * will be constructed everytime when launch the application.
 */
class KNMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /*!
     * \brief Constrcut a KNMainWindow widget.
     * \param parent The parent widget.
     */
    explicit KNMainWindow(QWidget *parent = 0);

    /*!
     * \brief Set the left bar widget, the main window will host the widget.\n
     * This function must be called before the set content widget, this function
     * must be called before the set main widget. Or else the main window will
     * break down the the program via assert.\n
     * The main window will take the ownership of the left bar.
     * \param leftBar The left bar widget pointer.
     */
    void setLeftBar(QWidget *leftBar);

    /*!
     * \brief Set main widget to main window, main window will host the widget.
     * \n
     * This function must be called after set the left bar. Or else main window
     * will break down the program via assert.\n
     * Main window will take the ownership of the main widget.
     * \param widget The widget pointer.
     */
    void setMainWidget(QWidget *widget);

signals:
    /*!
     * \brief This signal is emitted when the main window is going to close.
     */
    void aboutToClose();

public slots:

protected:
    /*!
     * \brief Reimplemented from QMainWindow::closeEvent().
     */
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onActionFullScreen();

private:
    QSplitter *m_container;
    KNMailPluginBase *m_mainWidget;
    Qt::WindowStates m_originalWindowState;
    bool m_windowClosed;
};

#endif // KNMAINWINDOW_H
