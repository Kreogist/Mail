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

#ifndef KMMIMEFILE_H
#define KMMIMEFILE_H

#include "kmmimebase.h"

/*!
 * \brief The KMMimeFile class provides the ability to describe files in the
 * mime format. This object won't load the file content until called toString()
 * function.\n
 * If you have set the file path, it will update the file data every time. Or
 * else, please set the content name as the target file name and content as
 * target file content.
 */
class KMMimeFile : public KMMimeBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KMMimeFile object.
     * \param parent The parent object pointer.
     */
    explicit KMMimeFile(QObject *parent = 0);

    /*!
     * \brief Get file path at the local file.
     * \return The file path.
     */
    QString filePath();

    /*!
     * \brief Reimplemented from KMMimeBase::toString().
     */
    QString toString() Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Set the file path
     * \param filePath
     */
    void setFilePath(const QString &filePath);
};

#endif // KMMIMEFILE_H
