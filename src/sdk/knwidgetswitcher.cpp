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
#include <QAction>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "knwidgetswitcher.h"

#include <QDebug>

KNWidgetSwitcher::KNWidgetSwitcher(QWidget *parent) :
    QWidget(parent),
    m_movingAnimationGroup(new QParallelAnimationGroup(this)),
    m_inAnimation(generateAnimation()),
    m_outAnimation(generateAnimation()),
    m_currentIndex(-1),
    m_outWidgetIndex(-1),
    m_maxWidth(0),
    m_maxHeight(0)
{
    //Set properties.
    setContentsMargins(0,0,0,0);
    setFocusPolicy(Qt::StrongFocus);

    //Add the animation to animation group.
    m_movingAnimationGroup->addAnimation(m_inAnimation);
    m_movingAnimationGroup->addAnimation(m_outAnimation);
    //Configure the moving animation gourp.
    connect(m_movingAnimationGroup, &QParallelAnimationGroup::finished,
            this, &KNWidgetSwitcher::onActionMovingFinished);

    //Add the go previous and back actions.
    QAction *goPrevTab=new QAction(this);
    goPrevTab->setShortcut(QKeySequence::Back);
    goPrevTab->setShortcutContext(Qt::WidgetShortcut);
    connect(goPrevTab, &QAction::triggered,
            this, &KNWidgetSwitcher::onActionMoveToPrevious);
    addAction(goPrevTab);

    QAction *goNextTab=new QAction(this);
    goNextTab->setShortcut(QKeySequence::Forward);
    goNextTab->setShortcutContext(Qt::WidgetShortcut);
    connect(goNextTab, &QAction::triggered,
            this, &KNWidgetSwitcher::onActionMoveToNext);
    addAction(goNextTab);
}

void KNWidgetSwitcher::addWidget(QWidget *widget)
{
    //Ignore the null widget.
    if(widget==nullptr)
    {
        return;
    }
    //Change the widget relashionship.
    widget->setParent(this, Qt::Widget);
    //Update the size hint parameter.
    m_maxWidth=qMax(m_maxWidth, widget->width());
    m_maxHeight=qMax(m_maxHeight, widget->height());
    //If this is the first widget add to switcher, it should be visible, and it
    //should be the current widget.
    if(m_widgets.isEmpty())
    {
        //Set current index to be 0.
        m_currentIndex=0;
        //Resize the widget to the same size as the container, and set the
        //position of the widget.
        widget->setGeometry(0, 0, width(), height());
        //Set the visible and enabled states.
        widget->setVisible(true);
        widget->setEnabled(true);
    }
    else
    {
        //Hide and disable the widget.
        widget->setVisible(false);
        widget->setEnabled(false);
    }
    //Add the widget to the widget list.
    m_widgets.append(widget);
}

int KNWidgetSwitcher::currentIndex() const
{
    return m_currentIndex;
}

bool KNWidgetSwitcher::removeWidget(QWidget *widget)
{
    //Check the widget ID in the list.
    int widgetIndex=m_widgets.indexOf(widget);
    //If we cannot find the widget, ignore it.
    if(widgetIndex==-1)
    {
        return false;
    }
    //Check whether the current index is out of range.
    if(m_currentIndex>m_widgets.size())
    {
        //Make the current index to be the -1.
        m_currentIndex=m_widgets.size()-1;
    }
    return true;
}

void KNWidgetSwitcher::clear()
{
    //Clear all the widgets in the widgets list.
    m_widgets.clear();
    //Reset the current index.
    m_currentIndex=-1;
}

int KNWidgetSwitcher::count() const
{
    return m_widgets.size();
}

QWidget *KNWidgetSwitcher::widgetAt(const int &index)
{
    //Get the widget if the index is valid, or else return nullptr.
    return (index>-1 && index<m_widgets.size())?
                m_widgets.at(index):nullptr;
}

QWidget *KNWidgetSwitcher::currentWidget()
{
    //Get the widget at m_currentIndex.
    return widgetAt(m_currentIndex);
}

QSize KNWidgetSwitcher::sizeHint() const
{
    //Combine the maximum width and maximum height.
    return QSize(m_maxWidth, m_maxHeight);
}

void KNWidgetSwitcher::setCurrentIndex(const int &currentIndex)
{
    //Ensure the current index is in the range.
    Q_ASSERT(currentIndex>-1 && currentIndex<m_widgets.size());
    //Check the following things:
    //  1. The current index is right the index we want.
    //  2. There's a switching animation is running.
    //If any one of the condition is detected, ignore the requirement.
    if(currentIndex==m_currentIndex ||
            m_movingAnimationGroup->state()==QAbstractAnimation::Running)
    {
        return;
    }
    //Stop the animation group.
    m_movingAnimationGroup->stop();
    //Save the last widget index to out index, set the new current index.
    m_outWidgetIndex=m_currentIndex;
    m_currentIndex=currentIndex;
    //Get the current widget.
    QWidget *currentWidget=m_widgets.at(m_currentIndex);
    //Reset the animation target.
    m_inAnimation->setTargetObject(currentWidget);
    m_outAnimation->setTargetObject(m_widgets.at(m_outWidgetIndex));
    //Update the start and end value.
    updateAnimationPosition();
    //Prepare the widget.
    setWidgetVisible(m_currentIndex, true);
    //Raise the current widget.
    currentWidget->raise();
    //Emit start to move signal.
    emit moveStart();
    //Start animation.
    m_movingAnimationGroup->start();
}

void KNWidgetSwitcher::setCurrentWidget(int currentIndex)
{
    //Ensure the current index is in the range.
    Q_ASSERT(currentIndex>-1 && currentIndex<m_widgets.size());
    //Change the current index widget to be the target widget without animation.
    //Hide the widget.
    setWidgetVisible(m_currentIndex, false);
    //Reset the out widget index to invalid.
    m_outWidgetIndex=-1;
    //Save current index.
    m_currentIndex=currentIndex;
    //Show the widget.
    setWidgetVisible(m_currentIndex, true);
    //Move the current widget.
    m_widgets.at(m_currentIndex)->setGeometry(rect());
}

void KNWidgetSwitcher::resizeEvent(QResizeEvent *event)
{
    //Do original resize.
    QWidget::resizeEvent(event);
    //If the animation is running, update the animation position.
    if(m_movingAnimationGroup->state()==QPropertyAnimation::Running)
    {
        updateAnimationPosition();
        return;
    }
    //Resize the current widget.
    if(m_currentIndex!=-1)
    {
        //Resize the current widget to the same size as the switcher.
        m_widgets.at(m_currentIndex)->resize(size());
    }
}

inline QPropertyAnimation *KNWidgetSwitcher::generateAnimation()
{
    //Generate the animation.
    QPropertyAnimation *animation=new QPropertyAnimation(this);
    //Set the animation properties.
    animation->setPropertyName("geometry");
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setDuration(200);
    //Return the animation.
    return animation;
}

inline void KNWidgetSwitcher::setWidgetVisible(const int &index, bool visible)
{
    Q_ASSERT(index>-1 && index<m_widgets.size());
    //Get the widget.
    QWidget *widget=m_widgets.at(index);
    //Set the widget visible.
    widget->setVisible(visible);
    widget->setEnabled(visible);
}

int KNWidgetSwitcher::outWidgetIndex() const
{
    return m_outWidgetIndex;
}

void KNWidgetSwitcher::onActionMovingFinished()
{
    //Emit move finished signal.
    emit moveEnd();
    //Check out whether widget index is invalid.
    if(m_outWidgetIndex<0 || (m_outWidgetIndex>m_widgets.size()))
    {
        //Ignore the calling.
        return;
    }
    //Hide the widget.
    setWidgetVisible(m_outWidgetIndex, false);
    //Reset the out widget index to invalid.
    m_outWidgetIndex=-1;
}

void KNWidgetSwitcher::onActionMoveToPrevious()
{
    //Check out the current index, if it's greater than the first one.
    if(m_currentIndex>0)
    {
        //Set current index to the next one.
        setCurrentIndex(m_currentIndex-1);
    }
}

void KNWidgetSwitcher::onActionMoveToNext()
{
    //Check out the current index, if it's less than the last one
    if(m_currentIndex<m_widgets.size()-1)
    {
        //Set current index to the previous one.
        setCurrentIndex(m_currentIndex+1);
    }
}

QPropertyAnimation *KNWidgetSwitcher::outAnimation() const
{
    return m_outAnimation;
}

QPropertyAnimation *KNWidgetSwitcher::inAnimation() const
{
    return m_inAnimation;
}
