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
#ifndef KNMAILVIEWERBASE_H
#define KNMAILVIEWERBASE_H

#include <QWidget>

class KNMailWebViewerBase;
/*!
 * \brief The KNMailViewerBase class provides the basic ports and functions
 * which a mail viewer should be provided. This port widget should be generate
 * by a factor of the viewer for multiple viewers.
 */
class KNMailViewerBase : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailViewerBase widget.
     * \param parent The parent widget.
     */
    KNMailViewerBase(QWidget *parent = 0) : QWidget(parent){}

signals:
    /*!
     * \brief Require to pop up the mail in a new window, only one viewer should
     *  emit this signal.
     * \param mailPath The mail path.
     */
    void requirePopup(const QString &mailPath);

    /*!
     * \brief When the window is closing, this signal will be emit to remove the
     * window from the popup list.
     * \param mailPath The mail file path.
     */
    void requireClose(const QString &mailPath);

public slots:
    /*!
     * \brief Set the web content viewer to the mail viewer.
     * \param The web viewer widget viewer pointer.
     */
    virtual void setWebViewer(KNMailWebViewerBase *viewer)=0;

    /*!
     * \brief Set the viewer to be a pop up window.
     * \param isPopup To make the window a pop up window, set this function to
     * be true.
     */
    virtual void setViewerPopup(bool isPopup)=0;

    /*!
     * \brief Load a mail from a file.
     * \param mailPath The mail file path.
     */
    virtual void loadMail(const QString &mailPath)=0;
};

#endif // KNMAILVIEWERBASE_H
