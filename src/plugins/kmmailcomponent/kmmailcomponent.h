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
#ifndef KMMAILCOMPONENT_H
#define KMMAILCOMPONENT_H

#include "kmmailcomponentbase.h"

class QScrollArea;
class QScrollBar;
class QTimeLine;
class KMMimePart;
class KMMailComponentTitleBar;
class KMMailComponentContentBase;
/*!
 * \brief The KMMailComponent class is an official provided KMMailComponentBase
 * realize. It uses all the sdk widgets to build.
 */
class KMMailComponent : public KMMailComponentBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMailComponent widget.
     * \param parent The parent widget.
     */
    explicit KMMailComponent(QWidget *parent = 0);
    ~KMMailComponent();

    void reset() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KMMailComponentBase::loadMail().
     */
    void loadMail(const QString &filePath) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KMMailComponentBase::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateGeometries();

private:
    inline void recoveryMimePart();
    KMMailComponentTitleBar *m_titleBar;
    KMMailComponentContentBase *m_content;
    KMMimePart *m_mimePart;
};

#endif // KMMAILCOMPONENT_H
