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
#ifndef KMMIMEPART_H
#define KMMIMEPART_H

#include <QMap>

#include <QObject>

class KMMimePart : public QObject
{
    Q_OBJECT
public:
    explicit KMMimePart(QObject *parent = 0);

    QMap<QString, QString> properties() const;

    bool hasMimeProperty(const QString &field) const;

    QString mimeProperty(const QString &field) const;

    virtual QByteArray content() const;

    virtual bool isMultipart() const;

signals:

public slots:
    void setMimeProperty(const QString name, const QString &data);
    void setMimeProperties(const QMap<QString, QString> &properties);
    virtual void setContent(const QByteArray &content);

protected:
    QMap<QString, QString> m_properties;
    QByteArray m_content;
};

#endif // KMMIMEPART_H
