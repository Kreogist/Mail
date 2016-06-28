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
#include <QApplication>
#include <QDesktopWidget>
#include <QTimeLine>

#include "knconfigure.h"
#include "knthememanager.h"
#include "kmmainwindowcontainer.h"
#include "kmglobal.h"
#include "kmcoverlayer.h"
#include "kmleftbarbase.h"
#include "kmmailcomponentbase.h"

#include "kmunibarbase.h"
#include "kmtitlebarbase.h"

#include "kmmainwindow.h"

#include <QDebug>

#define MaxOpacity 0xC0

KMMainWindow::KMMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_originalWindowState(Qt::WindowNoState),
    m_container(new KMMainWindowContainer(this)),
    m_floatLayer(new KMCoverLayer(this)),
    m_floatAnime(new QTimeLine(200, this)),
    m_titleBar(nullptr),
    m_leftBar(nullptr),
    m_uniBar(nullptr),
    m_cacheConfigure(kmGlobal->cacheConfigure()->getConfigure("MainWindow"))
{
    setObjectName("MainWindow");
    //Set properties.
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);
    setMinimumSize(730, 432);
    //Set the container as central widget.
    setCentralWidget(m_container);
    //Mac OS X title hack.
#ifdef Q_OS_MACX
    setWindowTitle(qApp->applicationDisplayName());
#endif
    //Add main window to theme list.
    knTheme->registerWidget(this);

    //Configure the second layer.
    m_floatLayer->hide();
    QPalette pal=m_floatLayer->palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,0));
    m_floatLayer->setPalette(pal);
    //Configure the time line.
    m_floatAnime->setUpdateInterval(10);
    m_floatAnime->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_floatAnime, &QTimeLine::frameChanged,
            this, &KMMainWindow::onActionShowHideFloatLayer);

    //Add full screen short cut actions.
    QAction *fullScreen=new QAction(this);
    fullScreen->setShortcut(QKeySequence(QKeySequence::FullScreen));
    connect(fullScreen, &QAction::triggered,
            this, &KMMainWindow::onActionFullScreen);
    addAction(fullScreen);
    //Recover the geometry.
    recoverGeometry();
}

void KMMainWindow::setTitleBar(KMTitleBarBase *titleBar)
{
    //Check pointer is already set or not.
    if(m_titleBar)
    {
        //Already has a title bar, ignore the second widget.
        return;
    }
    //Save the title bar.
    m_titleBar=titleBar;
    //Check title bar.
    if(!m_titleBar)
    {
        //Ignore null pointer.
        return;
    }
    //Set the title bar widget.
    m_container->setTitleBar(m_titleBar);
    //Link the title bar widget.
    connect(m_titleBar, &KMTitleBarBase::requireShowUnibar,
            this, &KMMainWindow::showUnibar);
    connect(m_titleBar, &KMTitleBarBase::requireShowPreference,
            this, &KMMainWindow::showPreference);
}

void KMMainWindow::setMailList(KMLeftBarBase *mailList)
{
    //Save the mail list.
    m_leftBar=mailList;
    //Set the left bar to container.
    m_container->setMailList(mailList);
}

void KMMainWindow::setUniBar(KMUnibarBase *uniBar)
{
//    m_container->setUniBar(uniBar);
    //Save the unibar widget first.
    m_uniBar=uniBar;
    //Configure the unibar.
    m_uniBar->setParent(this);
    //Set the title bar to unibar.
    m_uniBar->setTitleBar(m_titleBar);
    //Configure the unibar.
    m_uniBar->hide();
    m_uniBar->setShadowParent(m_floatLayer);
    //Link the unibar with the hide signal.
    connect(m_uniBar, &KMUnibarBase::switchModel,
            m_leftBar, &KMLeftBarBase::switchModel);
    connect(m_uniBar, &KMUnibarBase::switchModel,
            this, &KMMainWindow::onActionSwitchModel);
    connect(m_uniBar, &KMUnibarBase::requireUpdateTitle,
            m_titleBar, &KMTitleBarBase::setTitleText);
}

void KMMainWindow::setMailComponent(KMMailComponentBase *mailComponent)
{
    //Set the mail component widget.
    m_container->setMailComponent(mailComponent);
    //Link the mail component with the left bar.
    connect(m_leftBar, &KMLeftBarBase::requireLoadMail,
            mailComponent, &KMMailComponentBase::loadMail);
}

void KMMainWindow::setPreference(QWidget *preference)
{
    ;
}

void KMMainWindow::closeEvent(QCloseEvent *event)
{
    //Save the geometry.
    backupGeometry();
    //Do the mainwindow close event.
    QMainWindow::closeEvent(event);
}

void KMMainWindow::resizeEvent(QResizeEvent *event)
{
    //Resize the window first.
    QMainWindow::resizeEvent(event);
    //Update the layer size.
    m_floatLayer->resize(size());
    //Check unibar visible.
    if(m_uniBar->isVisible())
    {
        //Resize the unibar size.
        m_uniBar->resize(m_uniBar->width(), height());
    }
}

void KMMainWindow::onActionFullScreen()
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

void KMMainWindow::onActionShowHideFloatLayer(int frame)
{
    //Update the cover alpha.
    QPalette pal=m_floatLayer->palette();
    //Get and update alpha.
    QColor windowColor=pal.color(QPalette::Window);
    //Reset the alpha.
    windowColor.setAlpha(frame);
    //Update the window color.
    pal.setColor(QPalette::Window, windowColor);
    //Set the palette back.
    m_floatLayer->setPalette(pal);
}

void KMMainWindow::showUnibar()
{
    //Show the float layer.
    m_floatLayer->show();
    //Resize the float layer.
    m_floatLayer->resize(size());
    //Show the float layer.
    startAnime(MaxOpacity);
    //Link the cover layer to hide unibar.
    connect(m_floatLayer, &KMCoverLayer::clicked,
            this, &KMMainWindow::hideUnibar);
    //Show the unibar and launch the unibar animation.
    m_uniBar->show();
    m_uniBar->showUnibar(size());
}

void KMMainWindow::hideUnibar()
{
    //Launch hide unibar animation.
    m_uniBar->hideUnibar();
    //Disconnect the float layer signal.
    disconnect(m_floatLayer, &KMCoverLayer::clicked, 0, 0);
    //Link the anime finished signal.
    connect(m_floatAnime, &QTimeLine::finished,
            this, &KMMainWindow::onActionHideFloatLayerFinished);
    //Hide the float layer.
    startAnime(0);
}

void KMMainWindow::showPreference()
{
    //Show the float layer.
    m_floatLayer->show();
    //Resize the float layer.
    m_floatLayer->resize(size());
    //Show the float layer.
    startAnime(MaxOpacity);
    //Link the cover layer to hide unibar.
    connect(m_floatLayer, &KMCoverLayer::clicked,
            this, &KMMainWindow::hidePreference);
}

void KMMainWindow::hidePreference()
{
    //Disconnect the float layer signal.
    disconnect(m_floatLayer, &KMCoverLayer::clicked, 0, 0);
    //Link the anime finished signal.
    connect(m_floatAnime, &QTimeLine::finished,
            this, &KMMainWindow::onActionHidePreferenceFinished);
    //Hide the float layer.
    startAnime(0);
}

void KMMainWindow::onActionHideFloatLayerFinished()
{
    //Hide the cover layer.
    m_floatLayer->hide();
    //Disconnect the finished signal.
    disconnect(m_floatAnime, &QTimeLine::finished, 0, 0);
}

void KMMainWindow::onActionHidePreferenceFinished()
{
    //Hide the cover layer.
    m_floatLayer->hide();
    //Disconnect the finished signal.
    disconnect(m_floatAnime, &QTimeLine::finished, 0, 0);
}

void KMMainWindow::onActionSwitchModel()
{
    //Hide the unibar.
    hideUnibar();
}

inline void KMMainWindow::startAnime(int endFrame)
{
    //Stop anime time line.
    m_floatAnime->stop();
    //Set range.
    m_floatAnime->setFrameRange(
                m_floatLayer->palette().color(QPalette::Window).alpha(),
                endFrame);
    //Start anime.
    m_floatAnime->start();
}

inline void KMMainWindow::recoverGeometry()
{
    //Check is the last record is complete. If there's no windowWidth property
    //in the configure, means we don't save the last geometry. Ignore the
    //recover request.
    if(m_cacheConfigure->data("windowWidth").isNull())
    {
        return;
    }

    //Recover the window state.
    int windowState=m_cacheConfigure->data("windowState").toInt();
    //For maximum and full screen, we only need to set the window state.
    switch (windowState)
    {
    case Qt::WindowMaximized:
        setWindowState(Qt::WindowMaximized);
        return;
    case Qt::WindowFullScreen:
        setWindowState(Qt::WindowFullScreen);
        return;
    default:
        setWindowState(Qt::WindowNoState);
    }
    //Read the resolution data of the last time closed.
    int lastScreenWidth=getCacheValue("desktopWidth"),
        lastScreenHeight=getCacheValue("desktopHeight"),
        currentScreenWidth=qApp->desktop()->width(),
        currentScreenHeight=qApp->desktop()->height(),
        lastX=getCacheValue("windowX"),
        lastY=getCacheValue("windowY"),
        lastWidth=getCacheValue("windowWidth"),
        lastHeight=getCacheValue("windowHeight");
    //Check is the resolution is the same as the last closed time.
    if(!(lastScreenWidth==currentScreenWidth &&
         lastScreenHeight==currentScreenHeight))
    {
        //The resolution has been changed, recalculate the size parameters.
        //Get the width ratio and the height ratio.
        qreal widthRatio=(qreal)currentScreenWidth/(qreal)lastScreenWidth,
              heightRatio=(qreal)currentScreenHeight/(qreal)lastScreenHeight;
        //Recalculate the last parameters.
        zoomParameter(lastX, widthRatio);
        zoomParameter(lastY, heightRatio);
        zoomParameter(lastWidth, widthRatio);
        zoomParameter(lastHeight, heightRatio);
    }
    //Check the parameter, ensure that one part of the window must be inside
    //screen.
    //If it's not inside the screen, make the top of the window 0.
    if(lastY<0 || lastY>currentScreenHeight)
    {
        lastY=0;
    }
    //Set the geometry.
    setGeometry(lastX, lastY, lastWidth, lastHeight);
}

inline void KMMainWindow::backupGeometry()
{
    //Set the window state.
    setCacheValue("windowState", static_cast<int>(windowState()));
    //Set the window position.
    setCacheValue("windowX", geometry().x());
    setCacheValue("windowY", geometry().y());
    setCacheValue("windowWidth", geometry().width());
    setCacheValue("windowHeight", geometry().height());
    //Set the current desktop size.
    setCacheValue("desktopWidth", qApp->desktop()->width());
    setCacheValue("desktopHeight", qApp->desktop()->height());
}

inline int KMMainWindow::getCacheValue(const QString &valueName)
{
    return m_cacheConfigure->data(valueName).toInt();
}

void KMMainWindow::setCacheValue(const QString &valueName, int value)
{
    m_cacheConfigure->setData(valueName, value);
}

void KMMainWindow::zoomParameter(int &parameter, const qreal &ratio)
{
    parameter=(qreal)parameter*ratio;
}
