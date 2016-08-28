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
#ifndef KNMAILPLUGIN_H
#define KNMAILPLUGIN_H

#include "knmailpluginbase.h"

class QPushButton;
class QStackedLayout;
class KNMailViewerBase;
class KNMailAccountList;
class KNMailEmptyHintBase;
class KNMailFolderViewerBase;
class KNMailWebViewerBase;
class KNMailViewerGeneratorBase;
class KNMailWebViewerGeneratorBase;
class KNMailComposeGeneratorBase;
/*!
 * \brief The KNMailPlugin class is a default implementation of the mail plugin.
 */
class KNMailPlugin : public KNMailPluginBase
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMailPlugin widget.
     * \param parent The parent widget.
     */
    explicit KNMailPlugin(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMailPluginBase::accountPanel().
     */
    QWidget *accountPanel() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailPluginBase::loadPlugins().
     */
    void loadPlugins() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailPluginBase::composeButton().
     */
    QWidget *composeButton() const Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMailPluginBase::saveAccount().
     */
    void saveAccount() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMailPluginBase::startWorking().
     */
    void startWorking() Q_DECL_OVERRIDE;

protected:

private slots:
    void retranslate();
    void onActionComposePressed();

private:
    inline void initialInfrastructure();
    void loadEmptyHint(KNMailEmptyHintBase *emptyHint);
    void loadFolderViewer(KNMailFolderViewerBase *folderViewer);
    void loadWebViewerGenerator(KNMailWebViewerGeneratorBase *generator);
    void loadMailViewerGenerator(KNMailViewerGeneratorBase *generator);
    void loadComposerGenerator(KNMailComposeGeneratorBase *generator);
    KNMailAccountList *m_leftBarContainer;
    QPushButton *m_composeButton;
    QStackedLayout *m_mainLayout;
};

#endif // KNMAILPLUGIN_H
