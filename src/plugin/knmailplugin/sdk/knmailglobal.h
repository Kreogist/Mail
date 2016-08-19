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
#ifndef KNMAILGLOBAL_H
#define KNMAILGLOBAL_H

#include <QHash>
#include <QPixmap>

#include "knmailutil.h"

#include <QObject>

using namespace MailUtil;

/*!
 * \def knMailGlobal
 * A global pointer referring to the unique mail global object.
 */
#define knMailGlobal (KNMailGlobal::instance())

class KNMailViewerBase;
class KNMailWebViewerBase;
class KNMailViewerGeneratorBase;
class KNMailWebViewerGeneratorBase;
/*!
 * \brief The KNMailGlobal class provides some public or share data between the
 * mail plugin inside. For public structures or shared functions, it will all be
 * used in KNMailUtil class.
 */
class KNMailGlobal : public QObject
{
    Q_OBJECT
public:
    ~KNMailGlobal();

    /*!
     * \brief Get the mail global class instance.
     * \return The global instance of the KNMailGlobal.
     */
    static KNMailGlobal *instance();

    /*!
     * \brief Initial the mail global instance.
     * \param parent The parent of the instance.
     */
    static void initial(QObject *parent = 0);

    /*!
     * \brief Get the default folder name. It will be translate via the
     * different language.
     * \param index The folder index, it will be check before calling. It should
     * be no less than 0 and less than DefaultFolderCount.
     * \return Translated default folder name.
     */
    QString defaultFolderName(int index);

    /*!
     * \brief Get the viewer title text according to the index.
     * \param index Title viewer text index. Must in range of enumeration
     * MailViewerTitleField.
     * \return The title viewer text.
     */
    QString viewerTitleText(int index);

    /*!
     * \brief Get the provider icon.
     * \param index The provider index.
     * \return The icon of the E-mail provider.
     */
    QPixmap providerIcon(const QString &providerName);

    /*!
     * \brief Generate an mail viewer widget via the viewer generator.
     * \param parent The parent widget. Default is NULL. When it is NULL, the
     * parent will be the default parent.
     * \return The generated mail viewer pointer. If the viewer generator is not
     *  set, this function will always return nullptr.
     */
    KNMailViewerBase *generateViewer(QWidget *parent=nullptr);

    /*!
     * \brief Generate a popup mail viewer widget via the viewer generator.
     * \param parent The parent widget. Default is NULL. When it is NULL, the
     * parent of viewer will be the default parent.
     * \return The viewer pointer. If the viewer generator is not set, this
     * function will always return nullptr.
     */
    KNMailViewerBase *generatePopupViewer(QWidget *parent=nullptr);

    /*!
     * \brief Set the viewer generator factory pointer.
     * \param viewerGenerator The viewer generator pointer.
     * \param viewerParent The default parent of the generated viewer.
     */
    void setViewerGenerator(KNMailViewerGeneratorBase *viewerGenerator,
                            QWidget *viewerParent);

    /*!
     * \brief Generate a web viewer widget via web viewer generator factory
     * pointer.
     * \param parent The parent widget.
     * \return The web viewer pointer.
     */
    KNMailWebViewerBase *generateWebViewer(QWidget *parent);

    /*!
     * \brief Set the web viewer generator object.
     * \param webViewerGenerator The web viewer generator object.
     */
    void setWebViewerGenerator(
            KNMailWebViewerGeneratorBase *webViewerGenerator);

signals:

public slots:

private slots:
    void retranslate();

private:
    static KNMailGlobal *m_instance;
    explicit KNMailGlobal(QObject *parent = 0);
    KNMailGlobal(const KNMailGlobal &);
    KNMailGlobal(KNMailGlobal &&);

    QString m_defaultFolderName[DefaultFolderCount],
            m_titleFieldText[MailViewerTitleFieldCount];
    QHash<QString, QPixmap> m_providerIcon;
    KNMailViewerGeneratorBase *m_viewerGenerator;
    KNMailWebViewerGeneratorBase *m_webViewerGenerator;
    QWidget *m_viewerParent;
};

#endif // KNMAILGLOBAL_H
