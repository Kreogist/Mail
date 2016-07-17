# Copyright (C) Kreogist Dev Team
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

# Basic informations about the main mu project.
TEMPLATE = app
TARGET = mail

# Add Qt modules, the Qt major version should be greater than 5.
QT += \
    core \
    gui \
    widgets \
    xml \
    network

# Enabled C++ 11 configures.
CONFIG += c++11

# Add translation paths for Qt Linguist to generate translation files.
TRANSLATIONS += \
    i18n/Simplified_Chinese.ts \
    i18n/Traditional_Chinese.ts \
    i18n/Japanese.ts

intel64: {
    msvc: {
    # Intel Processor, Sandy bridge or later.
    QMAKE_CXXFLAGS_RELEASE += -favor:INTEL64
    # CPU Instrcutions.
    QMAKE_CXXFLAGS_RELEASE += -arch:AVX
    }
}

amd64: {
    msvc: {
    # AMD Processor,
    QMAKE_CXXFLAGS_RELEASE += -favor:AMD64
    # CPU Instrcutions.
    QMAKE_CXXFLAGS_RELEASE += -arch:AVX
    }
}

# Enabled processor instruction sets compile switches when using release mode
# for different compilers.
msvc: {
    # MSVC will only used for Windows 64-bit version.
    # Whole Program Optimization
    QMAKE_CXXFLAGS_RELEASE += -GL -Gw -GA
    # Auto-Parallelizer
    QMAKE_CXXFLAGS_RELEASE += -Qpar

    # Linker Options.
    # Link-time Code Generation, use with -GL
    QMAKE_LFLAGS_RELEASE += -LTCG
}
gcc: {
    # Instruction optimized
    CONFIG += mmx sse sse2 sse3
    QMAKE_CXXFLAGS_RELEASE += -mmmx -msse -msse2 -msse3
    # Vectorize optimization
    QMAKE_CXXFLAGS_RELEASE += -ftree-vectorize
    # Concurrency
    QMAKE_CXXFLAGS_RELEASE += -funroll-loops
    # Qt will use clang on Mac OS X, but it seems that it treats the clang as
    # gcc, it will use all the flags we set to gcc on clang. In clang, it
    # doesn't have -fforce-addr switch, so we need to set the switcher
    # especially for Windows and Linux.
    win32:{
        # Instruction optimized
        QMAKE_CXXFLAGS_RELEASE += -finline-functions
        # Vectorize optimization
        QMAKE_CXXFLAGS_RELEASE += -fivopts -ftree-vectorizer-verbose=2
        # Concurrency
        QMAKE_CXXFLAGS_RELEASE += -floop-parallelize-all
        # Cache optimization.
        QMAKE_CXXFLAGS_RELEASE += -fbranch-target-load-optimize
        # Loop optimization
        QMAKE_CXXFLAGS_RELEASE += -ftree-loop-linear
        # GCC will only used for Windows 32-bit version.
        QMAKE_CXXFLAGS_RELEASE += -fforce-addr
    }
    linux:{
        # Vectorize optimization
        QMAKE_CXXFLAGS_RELEASE += -fivopts -ftree-vectorizer-verbose=2
        # Loop optimization
        QMAKE_CXXFLAGS_RELEASE += -ftree-loop-linear
        # Cache optimization.
        QMAKE_CXXFLAGS_RELEASE += -fbranch-target-load-optimize
        # Concurrency
        QMAKE_CXXFLAGS_RELEASE += -floop-parallelize-all
        # GCC will only used for Linux 64-bit version.
        # Enabled force-addr.
        QMAKE_CXXFLAGS_RELEASE += -fforce-addr
        # Use sse to calculate the float operation.
        QMAKE_CXXFLAGS_RELEASE += -mfpmath=sse
        # Others
        QMAKE_CXXFLAGS_RELEASE += -ftracer
    }
}

# Platform Specific Configuration.
win32: {
    # Application icon.
    RC_FILE += resource/icon/win_resource.rc \
               resource/icon/mail.ico
    ICON = resource/icon/mu.ico
}

macx: {
    # Application icon.
    RC_FILE += resource/icon/mu.icns
    ICON = resource/icon/mu.icns
    # Set the info plist.
    QMAKE_INFO_PLIST = resource/platforms/mac/Info.plist
}

linux: {
    # Set the destination directory for the Linux special.
    DESTDIR = ../bin
    # This options is added for Linux specially.
    INSTALLS += target
}

# International Configureations.
i18n: {
    # MAKE_QM_FILES is a extra execution to called the lrelease to update the
    # qm file according to the original ts file real-time.
    MAKE_QM_FILES.input = TRANSLATIONS
    MAKE_QM_FILES.output = ${QMAKE_FILE_BASE}.qm
    MAKE_QM_FILES.commands = lrelease ${QMAKE_FILE_NAME} -qm ../bin/${QMAKE_FILE_BASE}.qm
    MAKE_QM_FILES.CONFIG += no_link target_predeps
    # Add MAKE_QM_FILES to the extra compilers.
    QMAKE_EXTRA_COMPILERS += MAKE_QM_FILES
}

# Add sdk directory to include path.
INCLUDEPATH += \
    sdk \
    plugin/knmailplugin/sdk

DISTFILES +=

RESOURCES += \
    resource/res.qrc

SOURCES += \
    main.cpp \
    sdk/knsingletonapplication.cpp \
    sdk/knmainwindow.cpp \
    core/knpluginmanager.cpp \
    sdk/knglobal.cpp \
    sdk/knconfigure.cpp \
    sdk/knconfiguremanager.cpp \
    sdk/knfontmanager.cpp \
    sdk/knlocalemanager.cpp \
    sdk/knthememanager.cpp \
    sdk/knutil.cpp \
    core/knmainwindowleftbar.cpp \
    plugin/knmailplugin/knmailplugin.cpp \
    plugin/knmailplugin/sdk/knmailglobal.cpp \
    plugin/knmailplugin/sdk/knmailutil.cpp \
    plugin/knmailplugin/sdk/knmailaccount.cpp \
    plugin/knmailplugin/sdk/knmailmodel.cpp \
    plugin/knmailplugin/sdk/knmailaccountmanager.cpp \
    plugin/knmailplugin/sdk/knmailaccountlist.cpp \
    sdk/knsensescrollarea.cpp \
    sdk/sao/knmessageboxblock.cpp \
    sdk/sao/knmessageboxcontent.cpp \
    sdk/sao/knsaobutton.cpp \
    sdk/sao/knsaostyle.cpp \
    sdk/sao/knsaosubmenu.cpp \
    plugin/knmailplugin/sdk/knmailaccountwidget.cpp \
    sdk/account/knaccountwidget.cpp \
    sdk/knhwidgetswitcher.cpp \
    sdk/knwidgetswitcher.cpp \
    sdk/account/knaccountutil.cpp \
    sdk/account/knaccountavatarbutton.cpp \
    sdk/account/knaccountloginpanel.cpp \
    sdk/knunderlinelineedit.cpp \
    plugin/knmailplugin/sdk/knmailaccountbutton.cpp \
    sdk/knopacityanimebutton.cpp \
    sdk/knopacitybutton.cpp \
    sdk/knsideshadowwidget.cpp \
    plugin/knmailplugin/plugin/knmailemptyhint/knmailemptyhint.cpp \
    plugin/knmailplugin/plugin/knmailfolderviewer/knmailfolderviewer.cpp \
    plugin/knmailplugin/plugin/knmailfolderviewer/knmailfolderviewertitle.cpp \
    plugin/knmailplugin/sdk/knmailtreeviewbase.cpp \
    plugin/knmailplugin/plugin/knmailfolderviewer/knmailfoldertreeview.cpp \
    sdk/knmousesenseheader.cpp \
    plugin/knmailplugin/plugin/knmailviewer/knmailviewer.cpp \
    plugin/knmailplugin/sdk/knmailviewermanager.cpp

HEADERS += \
    sdk/knsingletonapplication.h \
    sdk/knmainwindow.h \
    core/knpluginmanager.h \
    sdk/knversion.h \
    sdk/knglobal.h \
    sdk/knconfigure.h \
    sdk/knconfiguremanager.h \
    sdk/knfontmanager.h \
    sdk/knlocalemanager.h \
    sdk/knthememanager.h \
    sdk/knutil.h \
    sdk/knmainwindowleftbarbase.h \
    core/knmainwindowleftbar.h \
    sdk/knmailpluginbase.h \
    plugin/knmailplugin/knmailplugin.h \
    plugin/knmailplugin/sdk/knmailglobal.h \
    sdk/knaccountpanelbase.h \
    plugin/knmailplugin/sdk/knmailutil.h \
    plugin/knmailplugin/sdk/knmailaccount.h \
    plugin/knmailplugin/sdk/knmailmodel.h \
    plugin/knmailplugin/sdk/knmailaccountmanager.h \
    plugin/knmailplugin/sdk/knmailaccountlist.h \
    sdk/knsensescrollarea.h \
    sdk/sao/knmessageboxblock.h \
    sdk/sao/knmessageboxcontent.h \
    sdk/sao/knsaobutton.h \
    sdk/sao/knsaostyle.h \
    sdk/sao/knsaosubmenu.h \
    plugin/knmailplugin/sdk/knmailaccountwidget.h \
    sdk/account/knaccountwidget.h \
    sdk/knhwidgetswitcher.h \
    sdk/knwidgetswitcher.h \
    sdk/account/knaccountutil.h \
    sdk/account/knaccountavatarbutton.h \
    sdk/account/knaccountloginpanel.h \
    sdk/account/knaccountpanel.h \
    sdk/knunderlinelineedit.h \
    plugin/knmailplugin/sdk/knmailaccountbutton.h \
    sdk/knopacityanimebutton.h \
    sdk/knopacitybutton.h \
    sdk/knsideshadowwidget.h \
    plugin/knmailplugin/sdk/knmailemptyhintbase.h \
    plugin/knmailplugin/plugin/knmailemptyhint/knmailemptyhint.h \
    plugin/knmailplugin/sdk/knmailfolderviewerbase.h \
    plugin/knmailplugin/plugin/knmailfolderviewer/knmailfolderviewer.h \
    plugin/knmailplugin/plugin/knmailfolderviewer/knmailfolderviewertitle.h \
    plugin/knmailplugin/sdk/knmailtreeviewbase.h \
    plugin/knmailplugin/plugin/knmailfolderviewer/knmailfoldertreeview.h \
    sdk/knmousesenseheader.h \
    plugin/knmailplugin/sdk/knmailviewerbase.h \
    plugin/knmailplugin/sdk/knmailcomposebase.h \
    plugin/knmailplugin/plugin/knmailviewer/knmailviewer.h \
    plugin/knmailplugin/sdk/knmailviewermanager.h
