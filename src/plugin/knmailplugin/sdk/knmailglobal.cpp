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
#include "knlocalemanager.h"
#include "knglobal.h"

#include "knmailprotocolmanager.h"
#include "knmailcomposegeneratorbase.h"
#include "knmailaccount.h"
#include "knmailpopupmanager.h"
#include "knmailaccountmanager.h"
#include "knmailviewergeneratorbase.h"
#include "knmailviewerbase.h"
#include "knmailwebviewergeneratorbase.h"
#include "knmailcomposermanager.h"
#include "knmailreceivermanager.h"
#include "knmailsendermanager.h"
#include "knmailmodelupdater.h"

#include "knmailglobal.h"

#include <QDebug>

KNMailGlobal *KNMailGlobal::m_instance=nullptr;

KNMailGlobal::~KNMailGlobal()
{
    //Check the mail viewer generator pointer.
    if(m_viewerGenerator)
    {
        //Recover the memory.
        delete m_viewerGenerator;
        //Reset the pointer.
        m_viewerGenerator=nullptr;
    }
    //Check the web content generator pointer.
    if(m_webViewerGenerator)
    {
        //Recover the memory.
        delete m_webViewerGenerator;
        //Reset the pointer.
        m_webViewerGenerator=nullptr;
    }
    //Check the composer generator pointer.
    if(m_viewerGenerator)
    {
        //Recover the memory.
        delete m_viewerGenerator;
        //Reset the pointer.
        m_viewerGenerator=nullptr;
    }
    //Quit and wait the receiver thread.
    m_receiverThread.quit();
    m_receiverThread.wait();
    m_senderThread.quit();
    m_senderThread.wait();
    m_updaterThread.quit();
    m_updaterThread.wait();
    //Remove the receiver and sender.
    knMailReceiverManager->deleteLater();
    knMailSenderManager->deleteLater();
    //Recover the protocol manager object.
    knMailProtocolManager->deleteLater();
    //Recover the mail folder updater.
    knMailModelUpdater->deleteLater();
}

KNMailGlobal *KNMailGlobal::instance()
{
    return m_instance;
}

void KNMailGlobal::initial(QObject *parent)
{
    //Check if the singleton instance variable is null. Set the pointer to this
    //object if this is the first constructed object.
    if(m_instance==nullptr)
    {
        m_instance=new KNMailGlobal(parent);
    }
}

QString KNMailGlobal::defaultFolderDisplayName(int index) const
{
    Q_ASSERT(index>-1 && index<DefaultFolderCount);
    //Give back the folder name.
    return m_defaultFolderName[index];
}

QString KNMailGlobal::defaultFolderName(int index) const
{
    Q_ASSERT(index>-1 && index<DefaultFolderCount);
    //Give back the folder name.
    switch(index)
    {
    case FolderInbox:
        return "Inbox";
    case FolderDrafts:
        return "Drafts";
    case FolderSentItems:
        return "Sent";
    case FolderJunk:
        return "Junk";
    case FolderTrash:
        return "Trash";
    }
    return QString();
}

QString KNMailGlobal::mailAccountFolder() const
{
    return knGlobal->dirPath(KNGlobal::UserDataDir) + "/Accounts";
}

QString KNMailGlobal::viewerTitleText(int index)
{
    Q_ASSERT(index>-1 && index<MailViewerTitleFieldCount);
    //Give back the title field name.
    return m_titleFieldText[index];
}

QPixmap KNMailGlobal::providerIcon(const QString &providerName)
{
    return m_providerIcon.value(providerName, m_emtpyIcon);
}

KNMailViewerBase *KNMailGlobal::generateViewer()
{
    //Generate the viewer.
    KNMailViewerBase *mailViewer=m_viewerGenerator->generateViewer();
    //Add an web viewer.
    mailViewer->setWebViewer(generateWebViewer(mailViewer));
    //Return the viewer.
    return mailViewer;
}

KNMailViewerBase *KNMailGlobal::generatePopupViewer()
{
    //Generate the popup viewer.
    KNMailViewerBase *mailViewer=m_viewerGenerator->generatePopupViewer();
    //Add an web viewer.
    mailViewer->setWebViewer(generateWebViewer(mailViewer));
    //Return the viewer.
    return mailViewer;
}

void KNMailGlobal::retranslate()
{
    //Update the default folder name.
    m_defaultFolderName[FolderInbox]=tr("Inbox");
    m_defaultFolderName[FolderDrafts]=tr("Drafts");
    m_defaultFolderName[FolderSentItems]=tr("Sent");
    m_defaultFolderName[FolderJunk]=tr("Junk");
    m_defaultFolderName[FolderTrash]=tr("Trash");
    //Update the title field text.
    m_titleFieldText[FieldFrom]=tr("From");
    m_titleFieldText[FieldReceive]=tr("To");
    m_titleFieldText[FieldCarbonCopy]=tr("CC");
    //Update the subject.
    m_noSubject=tr("No subject");
}

KNMailGlobal::KNMailGlobal(QObject *parent) :
    QObject(parent),
    m_emtpyIcon(QPixmap(":/plugin/mail/providers/generic.png")),
    m_viewerGenerator(nullptr),
    m_webViewerGenerator(nullptr),
    m_composerGenerator(nullptr)
{
    //Register cached types.
    qRegisterMetaType<QVector<int>>("QVector<int>");

    //Initial the infrastructures.
    KNMailAccountManager::initial(this);
    KNMailPopupManager::initial(this);
    KNMailComposerManager::initial(this);
    KNMailModelUpdater::initial(&m_updaterThread);
    KNMailReceiverManager::initial(&m_receiverThread);
    KNMailSenderManager::initial(&m_senderThread);
    KNMailProtocolManager::initial();

    m_receiverThread.start();
    m_senderThread.start();

    //Construct the extension table.
    m_contentTypeExtension.insert("image/tiff", ".tif");
    m_contentTypeExtension.insert("application/x-001", ".001");
    m_contentTypeExtension.insert("application/x-301", ".301");
    m_contentTypeExtension.insert("text/h323", ".323");
    m_contentTypeExtension.insert("application/x-906", ".906");
    m_contentTypeExtension.insert("drawing/907", ".907");
    m_contentTypeExtension.insert("application/x-a11", ".a11");
    m_contentTypeExtension.insert("audio/x-mei-aac", ".acp");
    m_contentTypeExtension.insert("application/postscript", ".ai");
    m_contentTypeExtension.insert("audio/aiff", ".aif");
    m_contentTypeExtension.insert("audio/aiff", ".aifc");
    m_contentTypeExtension.insert("audio/aiff", ".aiff");
    m_contentTypeExtension.insert("application/x-anv", ".anv");
    m_contentTypeExtension.insert("text/asa", ".asa");
    m_contentTypeExtension.insert("video/x-ms-asf", ".asf");
    m_contentTypeExtension.insert("text/asp", ".asp");
    m_contentTypeExtension.insert("video/x-ms-asf", ".asx");
    m_contentTypeExtension.insert("audio/basic", ".au");
    m_contentTypeExtension.insert("video/avi", ".avi");
    m_contentTypeExtension.insert("application/vnd.adobe.workflow", ".awf");
    m_contentTypeExtension.insert("text/xml", ".biz");
    m_contentTypeExtension.insert("application/x-bmp", ".bmp");
    m_contentTypeExtension.insert("application/x-bot", ".bot");
    m_contentTypeExtension.insert("application/x-c4t", ".c4t");
    m_contentTypeExtension.insert("application/x-c90", ".c90");
    m_contentTypeExtension.insert("application/x-cals", ".cal");
    m_contentTypeExtension.insert("application/vnd.ms-pki.seccat", ".cat");
    m_contentTypeExtension.insert("application/x-netcdf", ".cdf");
    m_contentTypeExtension.insert("application/x-cdr", ".cdr");
    m_contentTypeExtension.insert("application/x-cel", ".cel");
    m_contentTypeExtension.insert("application/x-x509-ca-cert", ".cer");
    m_contentTypeExtension.insert("application/x-g4", ".cg4");
    m_contentTypeExtension.insert("application/x-cgm", ".cgm");
    m_contentTypeExtension.insert("application/x-cit", ".cit");
    m_contentTypeExtension.insert("java/*", ".class");
    m_contentTypeExtension.insert("text/xml", ".cml");
    m_contentTypeExtension.insert("application/x-cmp", ".cmp");
    m_contentTypeExtension.insert("application/x-cmx", ".cmx");
    m_contentTypeExtension.insert("application/x-cot", ".cot");
    m_contentTypeExtension.insert("application/pkix-crl", ".crl");
    m_contentTypeExtension.insert("application/x-x509-ca-cert", ".crt");
    m_contentTypeExtension.insert("application/x-csi", ".csi");
    m_contentTypeExtension.insert("text/css", ".css");
    m_contentTypeExtension.insert("application/x-cut", ".cut");
    m_contentTypeExtension.insert("application/x-dbf", ".dbf");
    m_contentTypeExtension.insert("application/x-dbm", ".dbm");
    m_contentTypeExtension.insert("application/x-dbx", ".dbx");
    m_contentTypeExtension.insert("text/xml", ".dcd");
    m_contentTypeExtension.insert("application/x-dcx", ".dcx");
    m_contentTypeExtension.insert("application/x-x509-ca-cert", ".der");
    m_contentTypeExtension.insert("application/x-dgn", ".dgn");
    m_contentTypeExtension.insert("application/x-dib", ".dib");
    m_contentTypeExtension.insert("application/x-msdownload", ".dll");
    m_contentTypeExtension.insert("application/msword", ".doc");
    m_contentTypeExtension.insert("application/msword", ".dot");
    m_contentTypeExtension.insert("application/x-drw", ".drw");
    m_contentTypeExtension.insert("text/xml", ".dtd");
    m_contentTypeExtension.insert("Model/vnd.dwf", ".dwf");
    m_contentTypeExtension.insert("application/x-dwf", ".dwf");
    m_contentTypeExtension.insert("application/x-dwg", ".dwg");
    m_contentTypeExtension.insert("application/x-dxb", ".dxb");
    m_contentTypeExtension.insert("application/x-dxf", ".dxf");
    m_contentTypeExtension.insert("application/vnd.adobe.edn", ".edn");
    m_contentTypeExtension.insert("application/x-emf", ".emf");
    m_contentTypeExtension.insert("message/rfc822", ".eml");
    m_contentTypeExtension.insert("text/xml", ".ent");
    m_contentTypeExtension.insert("application/x-epi", ".epi");
    m_contentTypeExtension.insert("application/x-ps", ".eps");
    m_contentTypeExtension.insert("application/postscript", ".eps");
    m_contentTypeExtension.insert("application/x-ebx", ".etd");
    m_contentTypeExtension.insert("application/x-msdownload", ".exe");
    m_contentTypeExtension.insert("image/fax", ".fax");
    m_contentTypeExtension.insert("application/vnd.fdf", ".fdf");
    m_contentTypeExtension.insert("application/fractals", ".fif");
    m_contentTypeExtension.insert("text/xml", ".fo");
    m_contentTypeExtension.insert("application/x-frm", ".frm");
    m_contentTypeExtension.insert("application/x-g4", ".g4");
    m_contentTypeExtension.insert("application/x-gbr", ".gbr");
    m_contentTypeExtension.insert("application/x-", ".");
    m_contentTypeExtension.insert("image/gif", ".gif");
    m_contentTypeExtension.insert("application/x-gl2", ".gl2");
    m_contentTypeExtension.insert("application/x-gp4", ".gp4");
    m_contentTypeExtension.insert("application/x-hgl", ".hgl");
    m_contentTypeExtension.insert("application/x-hmr", ".hmr");
    m_contentTypeExtension.insert("application/x-hpgl", ".hpg");
    m_contentTypeExtension.insert("application/x-hpl", ".hpl");
    m_contentTypeExtension.insert("application/mac-binhex40", ".hqx");
    m_contentTypeExtension.insert("application/x-hrf", ".hrf");
    m_contentTypeExtension.insert("application/hta", ".hta");
    m_contentTypeExtension.insert("text/x-component", ".htc");
    m_contentTypeExtension.insert("text/html", ".htm");
    m_contentTypeExtension.insert("text/html", ".html");
    m_contentTypeExtension.insert("text/webviewhtml", ".htt");
    m_contentTypeExtension.insert("text/html", ".htx");
    m_contentTypeExtension.insert("application/x-icb", ".icb");
    m_contentTypeExtension.insert("image/x-icon", ".ico");
    m_contentTypeExtension.insert("application/x-ico", ".ico");
    m_contentTypeExtension.insert("application/x-iff", ".iff");
    m_contentTypeExtension.insert("application/x-g4", ".ig4");
    m_contentTypeExtension.insert("application/x-igs", ".igs");
    m_contentTypeExtension.insert("application/x-iphone", ".iii");
    m_contentTypeExtension.insert("application/x-img", ".img");
    m_contentTypeExtension.insert("application/x-internet-signup", ".ins");
    m_contentTypeExtension.insert("application/x-internet-signup", ".isp");
    m_contentTypeExtension.insert("video/x-ivf", ".IVF");
    m_contentTypeExtension.insert("java/*", ".java");
    m_contentTypeExtension.insert("image/jpeg", ".jfif");
    m_contentTypeExtension.insert("image/jpeg", ".jpe");
    m_contentTypeExtension.insert("application/x-jpe", ".jpe");
    m_contentTypeExtension.insert("image/jpeg", ".jpeg");
    m_contentTypeExtension.insert("image/jpeg", ".jpg");
    m_contentTypeExtension.insert("application/x-jpg", ".jpg");
    m_contentTypeExtension.insert("application/x-javascript", ".js");
    m_contentTypeExtension.insert("text/html", ".jsp");
    m_contentTypeExtension.insert("audio/x-liquid-file", ".la1");
    m_contentTypeExtension.insert("application/x-laplayer-reg", ".lar");
    m_contentTypeExtension.insert("application/x-latex", ".latex");
    m_contentTypeExtension.insert("audio/x-liquid-secure", ".lavs");
    m_contentTypeExtension.insert("application/x-lbm", ".lbm");
    m_contentTypeExtension.insert("audio/x-la-lms", ".lmsff");
    m_contentTypeExtension.insert("application/x-javascript", ".ls");
    m_contentTypeExtension.insert("application/x-ltr", ".ltr");
    m_contentTypeExtension.insert("video/x-mpeg", ".m1v");
    m_contentTypeExtension.insert("video/x-mpeg", ".m2v");
    m_contentTypeExtension.insert("audio/mpegurl", ".m3u");
    m_contentTypeExtension.insert("video/mpeg4", ".m4e");
    m_contentTypeExtension.insert("application/x-mac", ".mac");
    m_contentTypeExtension.insert("application/x-troff-man", ".man");
    m_contentTypeExtension.insert("text/xml", ".math");
    m_contentTypeExtension.insert("application/msaccess", ".mdb");
    m_contentTypeExtension.insert("application/x-mdb", ".mdb");
    m_contentTypeExtension.insert("application/x-shockwave-flash", ".mfp");
    m_contentTypeExtension.insert("message/rfc822", ".mht");
    m_contentTypeExtension.insert("message/rfc822", ".mhtml");
    m_contentTypeExtension.insert("application/x-mi", ".mi");
    m_contentTypeExtension.insert("audio/mid", ".mid");
    m_contentTypeExtension.insert("audio/mid", ".midi");
    m_contentTypeExtension.insert("application/x-mil", ".mil");
    m_contentTypeExtension.insert("text/xml", ".mml");
    m_contentTypeExtension.insert("audio/x-musicnet-download", ".mnd");
    m_contentTypeExtension.insert("audio/x-musicnet-stream", ".mns");
    m_contentTypeExtension.insert("application/x-javascript", ".mocha");
    m_contentTypeExtension.insert("video/x-sgi-movie", ".movie");
    m_contentTypeExtension.insert("audio/mp1", ".mp1");
    m_contentTypeExtension.insert("audio/mp2", ".mp2");
    m_contentTypeExtension.insert("video/mpeg", ".mp2v");
    m_contentTypeExtension.insert("audio/mp3", ".mp3");
    m_contentTypeExtension.insert("video/mpeg4", ".mp4");
    m_contentTypeExtension.insert("video/x-mpg", ".mpa");
    m_contentTypeExtension.insert("application/vnd.ms-project", ".mpd");
    m_contentTypeExtension.insert("video/x-mpeg", ".mpe");
    m_contentTypeExtension.insert("video/mpg", ".mpeg");
    m_contentTypeExtension.insert("video/mpg", ".mpg");
    m_contentTypeExtension.insert("audio/rn-mpeg", ".mpga");
    m_contentTypeExtension.insert("application/vnd.ms-project", ".mpp");
    m_contentTypeExtension.insert("video/x-mpeg", ".mps");
    m_contentTypeExtension.insert("application/vnd.ms-project", ".mpt");
    m_contentTypeExtension.insert("video/mpg", ".mpv");
    m_contentTypeExtension.insert("video/mpeg", ".mpv2");
    m_contentTypeExtension.insert("application/vnd.ms-project", ".mpw");
    m_contentTypeExtension.insert("application/vnd.ms-project", ".mpx");
    m_contentTypeExtension.insert("text/xml", ".mtx");
    m_contentTypeExtension.insert("application/x-mmxp", ".mxp");
    m_contentTypeExtension.insert("image/pnetvue", ".net");
    m_contentTypeExtension.insert("application/x-nrf", ".nrf");
    m_contentTypeExtension.insert("message/rfc822", ".nws");
    m_contentTypeExtension.insert("text/x-ms-odc", ".odc");
    m_contentTypeExtension.insert("application/x-out", ".out");
    m_contentTypeExtension.insert("application/pkcs10", ".p10");
    m_contentTypeExtension.insert("application/x-pkcs12", ".p12");
    m_contentTypeExtension.insert("application/x-pkcs7-certificates", ".p7b");
    m_contentTypeExtension.insert("application/pkcs7-mime", ".p7c");
    m_contentTypeExtension.insert("application/pkcs7-mime", ".p7m");
    m_contentTypeExtension.insert("application/x-pkcs7-certreqresp", ".p7r");
    m_contentTypeExtension.insert("application/pkcs7-signature", ".p7s");
    m_contentTypeExtension.insert("application/x-pc5", ".pc5");
    m_contentTypeExtension.insert("application/x-pci", ".pci");
    m_contentTypeExtension.insert("application/x-pcl", ".pcl");
    m_contentTypeExtension.insert("application/x-pcx", ".pcx");
    m_contentTypeExtension.insert("application/pdf", ".pdf");
    m_contentTypeExtension.insert("application/pdf", ".pdf");
    m_contentTypeExtension.insert("application/vnd.adobe.pdx", ".pdx");
    m_contentTypeExtension.insert("application/x-pkcs12", ".pfx");
    m_contentTypeExtension.insert("application/x-pgl", ".pgl");
    m_contentTypeExtension.insert("application/x-pic", ".pic");
    m_contentTypeExtension.insert("application/vnd.ms-pki.pko", ".pko");
    m_contentTypeExtension.insert("application/x-perl", ".pl");
    m_contentTypeExtension.insert("text/html", ".plg");
    m_contentTypeExtension.insert("audio/scpls", ".pls");
    m_contentTypeExtension.insert("application/x-plt", ".plt");
    m_contentTypeExtension.insert("image/png", ".png");
    m_contentTypeExtension.insert("application/x-png", ".png");
    m_contentTypeExtension.insert("application/vnd.ms-powerpoint", ".pot");
    m_contentTypeExtension.insert("application/vnd.ms-powerpoint", ".ppa");
    m_contentTypeExtension.insert("application/x-ppm", ".ppm");
    m_contentTypeExtension.insert("application/vnd.ms-powerpoint", ".pps");
    m_contentTypeExtension.insert("application/vnd.ms-powerpoint", ".ppt");
    m_contentTypeExtension.insert("application/x-ppt", ".ppt");
    m_contentTypeExtension.insert("application/x-pr", ".pr");
    m_contentTypeExtension.insert("application/pics-rules", ".prf");
    m_contentTypeExtension.insert("application/x-prn", ".prn");
    m_contentTypeExtension.insert("application/x-prt", ".prt");
    m_contentTypeExtension.insert("application/x-ps", ".ps");
    m_contentTypeExtension.insert("application/postscript", ".ps");
    m_contentTypeExtension.insert("application/x-ptn", ".ptn");
    m_contentTypeExtension.insert("application/vnd.ms-powerpoint", ".pwz");
    m_contentTypeExtension.insert("text/vnd.rn-realtext3d", ".r3t");
    m_contentTypeExtension.insert("audio/vnd.rn-realaudio", ".ra");
    m_contentTypeExtension.insert("audio/x-pn-realaudio", ".ram");
    m_contentTypeExtension.insert("application/x-ras", ".ras");
    m_contentTypeExtension.insert("application/rat-file", ".rat");
    m_contentTypeExtension.insert("text/xml", ".rdf");
    m_contentTypeExtension.insert("application/vnd.rn-recording", ".rec");
    m_contentTypeExtension.insert("application/x-red", ".red");
    m_contentTypeExtension.insert("application/x-rgb", ".rgb");
    m_contentTypeExtension.insert("application/vnd.rn-realsystem-rjs", ".rjs");
    m_contentTypeExtension.insert("application/vnd.rn-realsystem-rjt", ".rjt");
    m_contentTypeExtension.insert("application/x-rlc", ".rlc");
    m_contentTypeExtension.insert("application/x-rle", ".rle");
    m_contentTypeExtension.insert("application/vnd.rn-realmedia", ".rm");
    m_contentTypeExtension.insert("application/vnd.adobe.rmf", ".rmf");
    m_contentTypeExtension.insert("audio/mid", ".rmi");
    m_contentTypeExtension.insert("application/vnd.rn-realsystem-rmj", ".rmj");
    m_contentTypeExtension.insert("audio/x-pn-realaudio", ".rmm");
    m_contentTypeExtension.insert("application/vnd.rn-rn_music_package", ".rmp");
    m_contentTypeExtension.insert("application/vnd.rn-realmedia-secure", ".rms");
    m_contentTypeExtension.insert("application/vnd.rn-realmedia-vbr", ".rmvb");
    m_contentTypeExtension.insert("application/vnd.rn-realsystem-rmx", ".rmx");
    m_contentTypeExtension.insert("application/vnd.rn-realplayer", ".rnx");
    m_contentTypeExtension.insert("image/vnd.rn-realpix", ".rp");
    m_contentTypeExtension.insert("audio/x-pn-realaudio-plugin", ".rpm");
    m_contentTypeExtension.insert("application/vnd.rn-rsml", ".rsml");
    m_contentTypeExtension.insert("text/vnd.rn-realtext", ".rt");
    m_contentTypeExtension.insert("application/msword", ".rtf");
    m_contentTypeExtension.insert("application/x-rtf", ".rtf");
    m_contentTypeExtension.insert("video/vnd.rn-realvideo", ".rv");
    m_contentTypeExtension.insert("application/x-sam", ".sam");
    m_contentTypeExtension.insert("application/x-sat", ".sat");
    m_contentTypeExtension.insert("application/sdp", ".sdp");
    m_contentTypeExtension.insert("application/x-sdw", ".sdw");
    m_contentTypeExtension.insert("application/x-stuffit", ".sit");
    m_contentTypeExtension.insert("application/x-slb", ".slb");
    m_contentTypeExtension.insert("application/x-sld", ".sld");
    m_contentTypeExtension.insert("drawing/x-slk", ".slk");
    m_contentTypeExtension.insert("application/smil", ".smi");
    m_contentTypeExtension.insert("application/smil", ".smil");
    m_contentTypeExtension.insert("application/x-smk", ".smk");
    m_contentTypeExtension.insert("audio/basic", ".snd");
    m_contentTypeExtension.insert("text/plain", ".sol");
    m_contentTypeExtension.insert("text/plain", ".sor");
    m_contentTypeExtension.insert("application/x-pkcs7-certificates", ".spc");
    m_contentTypeExtension.insert("application/futuresplash", ".spl");
    m_contentTypeExtension.insert("text/xml", ".spp");
    m_contentTypeExtension.insert("application/streamingmedia", ".ssm");
    m_contentTypeExtension.insert("application/vnd.ms-pki.certstore", ".sst");
    m_contentTypeExtension.insert("application/vnd.ms-pki.stl", ".stl");
    m_contentTypeExtension.insert("text/html", ".stm");
    m_contentTypeExtension.insert("application/x-sty", ".sty");
    m_contentTypeExtension.insert("text/xml", ".svg");
    m_contentTypeExtension.insert("application/x-shockwave-flash", ".swf");
    m_contentTypeExtension.insert("application/x-tdf", ".tdf");
    m_contentTypeExtension.insert("application/x-tg4", ".tg4");
    m_contentTypeExtension.insert("application/x-tga", ".tga");
    m_contentTypeExtension.insert("image/tiff", ".tif");
    m_contentTypeExtension.insert("application/x-tif", ".tif");
    m_contentTypeExtension.insert("image/tiff", ".tiff");
    m_contentTypeExtension.insert("text/xml", ".tld");
    m_contentTypeExtension.insert("drawing/x-top", ".top");
    m_contentTypeExtension.insert("application/x-bittorrent", ".torrent");
    m_contentTypeExtension.insert("text/xml", ".tsd");
    m_contentTypeExtension.insert("text/plain", ".txt");
    m_contentTypeExtension.insert("application/x-icq", ".uin");
    m_contentTypeExtension.insert("text/iuls", ".uls");
    m_contentTypeExtension.insert("text/x-vcard", ".vcf");
    m_contentTypeExtension.insert("application/x-vda", ".vda");
    m_contentTypeExtension.insert("application/vnd.visio", ".vdx");
    m_contentTypeExtension.insert("text/xml", ".vml");
    m_contentTypeExtension.insert("application/x-vpeg005", ".vpg");
    m_contentTypeExtension.insert("application/vnd.visio", ".vsd");
    m_contentTypeExtension.insert("application/x-vsd", ".vsd");
    m_contentTypeExtension.insert("application/vnd.visio", ".vss");
    m_contentTypeExtension.insert("application/vnd.visio", ".vst");
    m_contentTypeExtension.insert("application/x-vst", ".vst");
    m_contentTypeExtension.insert("application/vnd.visio", ".vsw");
    m_contentTypeExtension.insert("application/vnd.visio", ".vsx");
    m_contentTypeExtension.insert("application/vnd.visio", ".vtx");
    m_contentTypeExtension.insert("text/xml", ".vxml");
    m_contentTypeExtension.insert("audio/wav", ".wav");
    m_contentTypeExtension.insert("audio/x-ms-wax", ".wax");
    m_contentTypeExtension.insert("application/x-wb1", ".wb1");
    m_contentTypeExtension.insert("application/x-wb2", ".wb2");
    m_contentTypeExtension.insert("application/x-wb3", ".wb3");
    m_contentTypeExtension.insert("image/vnd.wap.wbmp", ".wbmp");
    m_contentTypeExtension.insert("application/msword", ".wiz");
    m_contentTypeExtension.insert("application/x-wk3", ".wk3");
    m_contentTypeExtension.insert("application/x-wk4", ".wk4");
    m_contentTypeExtension.insert("application/x-wkq", ".wkq");
    m_contentTypeExtension.insert("application/x-wks", ".wks");
    m_contentTypeExtension.insert("video/x-ms-wm", ".wm");
    m_contentTypeExtension.insert("audio/x-ms-wma", ".wma");
    m_contentTypeExtension.insert("application/x-ms-wmd", ".wmd");
    m_contentTypeExtension.insert("application/x-wmf", ".wmf");
    m_contentTypeExtension.insert("text/vnd.wap.wml", ".wml");
    m_contentTypeExtension.insert("video/x-ms-wmv", ".wmv");
    m_contentTypeExtension.insert("video/x-ms-wmx", ".wmx");
    m_contentTypeExtension.insert("application/x-ms-wmz", ".wmz");
    m_contentTypeExtension.insert("application/x-wp6", ".wp6");
    m_contentTypeExtension.insert("application/x-wpd", ".wpd");
    m_contentTypeExtension.insert("application/x-wpg", ".wpg");
    m_contentTypeExtension.insert("application/vnd.ms-wpl", ".wpl");
    m_contentTypeExtension.insert("application/x-wq1", ".wq1");
    m_contentTypeExtension.insert("application/x-wr1", ".wr1");
    m_contentTypeExtension.insert("application/x-wri", ".wri");
    m_contentTypeExtension.insert("application/x-wrk", ".wrk");
    m_contentTypeExtension.insert("application/x-ws", ".ws");
    m_contentTypeExtension.insert("application/x-ws", ".ws2");
    m_contentTypeExtension.insert("text/scriptlet", ".wsc");
    m_contentTypeExtension.insert("text/xml", ".wsdl");
    m_contentTypeExtension.insert("video/x-ms-wvx", ".wvx");
    m_contentTypeExtension.insert("application/vnd.adobe.xdp", ".xdp");
    m_contentTypeExtension.insert("text/xml", ".xdr");
    m_contentTypeExtension.insert("application/vnd.adobe.xfd", ".xfd");
    m_contentTypeExtension.insert("application/vnd.adobe.xfdf", ".xfdf");
    m_contentTypeExtension.insert("text/html", ".html");
    m_contentTypeExtension.insert("application/vnd.ms-excel", ".xls");
    m_contentTypeExtension.insert("application/x-xls", ".xls");
    m_contentTypeExtension.insert("application/x-xlw", ".xlw");
    m_contentTypeExtension.insert("text/xml", ".xml");
    m_contentTypeExtension.insert("audio/scpls", ".xpl");
    m_contentTypeExtension.insert("text/xml", ".xq");
    m_contentTypeExtension.insert("text/xml", ".xql");
    m_contentTypeExtension.insert("text/xml", ".xquery");
    m_contentTypeExtension.insert("text/xml", ".xsd");
    m_contentTypeExtension.insert("text/xml", ".xsl");
    m_contentTypeExtension.insert("text/xml", ".xslt");
    m_contentTypeExtension.insert("application/x-xwd", ".xwd");
    m_contentTypeExtension.insert("application/x-x_b", ".x_b");
    m_contentTypeExtension.insert("application/vnd.symbian.install", ".sis");
    m_contentTypeExtension.insert("application/vnd.symbian.install", ".sisx");
    m_contentTypeExtension.insert("application/x-x_t", ".x_t");
    m_contentTypeExtension.insert("application/vnd.iphone", ".ipa");
    m_contentTypeExtension.insert("application/vnd.android.package-archive", ".apk");
    m_contentTypeExtension.insert("application/x-silverlight-app", ".xap");

    //Initial the provider icons.
    m_providerIcon.insert("netease",
                          QPixmap(":/plugin/mail/providers/netease.png"));
    m_providerIcon.insert("google",
                          QPixmap(":/plugin/mail/providers/gmail.png"));

    //Link the update request.
    connect(this, &KNMailGlobal::requireUpdateAll,
            knMailReceiverManager, &KNMailReceiverManager::updateAllAccount);

    //Link retranslate.
    knI18n->link(this, &KNMailGlobal::retranslate);
    retranslate();
}

QString KNMailGlobal::noSubjectText() const
{
    return m_noSubject;
}

QString KNMailGlobal::contentExtension(const QString &contentType)
{
    return m_contentTypeExtension.value(contentType, ".txt");
}

void KNMailGlobal::parseContentType(const QString &rawData,
                                    QString &contentType,
                                    QMap<QString, QString> &attributes)
{
    //Split the raw data.
    QStringList typeAttributeList=rawData.split(';');
    //Check each attribute.
    for(QString i : typeAttributeList)
    {
        //Find '=' mark.
        int equalPosition=i.indexOf('=');
        //Check the position.
        if(equalPosition==-1)
        {
            //Save as type.
            contentType=i.simplified();
            //Move to next.
            continue;
        }
        //Parse the content.
        attributes.insert(i.left(equalPosition).toLower().simplified(),
                          i.mid(equalPosition+1).simplified());
    }
}

void KNMailGlobal::setComposerGenerator(
        KNMailComposeGeneratorBase *composerGenerator)
{
    m_composerGenerator = composerGenerator;
}

void KNMailGlobal::setWebViewerGenerator(
        KNMailWebViewerGeneratorBase *webViewerGenerator)
{
    m_webViewerGenerator = webViewerGenerator;
}

KNMailComposeBase *KNMailGlobal::generateComposer()
{
    return m_composerGenerator->generateComposer();
}

void KNMailGlobal::setViewerGenerator(
        KNMailViewerGeneratorBase *viewerGenerator)
{
    //Save the generator and parent.
    m_viewerGenerator = viewerGenerator;
}

KNMailWebViewerBase *KNMailGlobal::generateWebViewer(QWidget *parent)
{
    //Check the generator.
    return m_webViewerGenerator==nullptr?
                //Return NULL for not set the generator.
                nullptr:
                //Generate the web viewer.
                m_webViewerGenerator->generateWebViewer(parent);
}
