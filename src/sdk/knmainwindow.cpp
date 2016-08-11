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
#include <QAction>
#include <QSplitter>
#ifdef Q_OS_MACX
#include <QApplication>
#endif

#include "knthememanager.h"

#include "knmainwindow.h"

KNMainWindow::KNMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_container(new QSplitter(this)),
    m_originalWindowState(Qt::WindowNoState)
{
    setObjectName("MainWindow");
    //Set properties.
    setAutoFillBackground(true);
    setCentralWidget(m_container);
    setContentsMargins(0,0,0,0);
    setMinimumSize(853, 480);
    setWindowIcon(QIcon("://icon/mu.png"));
    //Configure the splitter.
    m_container->setHandleWidth(0);
    //Mac OS X title hack.
#ifdef Q_OS_MACX
    setWindowTitle(qApp->applicationDisplayName());
#endif

    //Add main window to theme list.
    knTheme->registerWidget(this);
    //Add full screen short cut actions.
#ifdef Q_OS_LINUX
    QAction *fullScreen=new QAction(this);
    fullScreen->setShortcut(QKeySequence(Qt::Key_F11));
    fullScreen->setShortcutContext(Qt::WindowShortcut);
    connect(fullScreen, &QAction::triggered,
            this, &KNMainWindow::onActionFullScreen);
    addAction(fullScreen);
#else
    QAction *fullScreen=new QAction(this);
    fullScreen->setShortcut(QKeySequence(QKeySequence::FullScreen));
    connect(fullScreen, &QAction::triggered,
            this, &KNMainWindow::onActionFullScreen);
    addAction(fullScreen);
#endif
}

void KNMainWindow::setLeftBar(QWidget *leftBar)
{
    //Check the container is empty or not.
    Q_ASSERT(m_container->count()==0);
    //Add widget to main layout.
    m_container->addWidget(leftBar);
}

void KNMainWindow::setMainWidget(QWidget *widget)
{
    //Check the container is empty or not.
    Q_ASSERT(m_container->count()!=0);
    //Add widget to main layout.
    m_container->addWidget(widget);
    //Update the strectch parameter.
    m_container->setStretchFactor(1, 1);
}

void KNMainWindow::onActionFullScreen()
{
    //Check out the full screen state.
    if(isFullScreen())
    {
        //Check the original window state.
        if(m_originalWindowState==Qt::WindowFullScreen)
        {
            //Set it to be no state.
            m_originalWindowState=Qt::WindowNoState;
        }
        //Set the window to normal state.
        setWindowState(m_originalWindowState);
    }
    else
    {
        //Save the original window state.
        m_originalWindowState=windowState();
        //Full screen the window.
        setWindowState(Qt::WindowFullScreen);
    }
}
