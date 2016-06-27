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
#ifndef KMMIMEMULTIPART_H
#define KMMIMEMULTIPART_H

#include "mime/kmmimepart.h"

class KMMimeMultiPart : public KMMimePart
{
    Q_OBJECT
public:
    explicit KMMimeMultiPart(QObject *parent = 0);
    ~KMMimeMultiPart();

    bool addMimePart(KMMimePart *mimePart);

    QByteArray content() const Q_DECL_OVERRIDE;

    bool isMultipart() const Q_DECL_OVERRIDE;

    KMMimePart *part(int i);

    int partCount() const;

signals:

public slots:
    void setContent(const QByteArray &content) Q_DECL_OVERRIDE;

private:
    QList<KMMimePart *> m_mimeParts;
};

#endif // KMMIMEMULTIPART_H
