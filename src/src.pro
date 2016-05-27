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
    network

# Enabled C++ 11 configures.
CONFIG += c++11

# Add sdk directory to include path.
INCLUDEPATH += \
    sdk

# 64-bit modern processor optimized switches.
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
    QMAKE_CXXFLAGS_RELEASE += -mmmx -msse -msse2 -msse3 -finline-functions
    # Vectorize optimization
    QMAKE_CXXFLAGS_RELEASE += -fivopts -ftree-vectorize
    # Concurrency
    QMAKE_CXXFLAGS_RELEASE += -funroll-loops
    # Qt will use clang on Mac OS X, but it seems that it treats the clang as
    # gcc, it will use all the flags we set to gcc on clang. In clang, it
    # doesn't have -fforce-addr switch, so we need to set the switcher
    # especially for Windows and Linux.
    win32:{
        # Vectorize optimization
        QMAKE_CXXFLAGS_RELEASE += -ftree-vectorizer-verbose=2
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
        QMAKE_CXXFLAGS_RELEASE += -ftree-vectorizer-verbose=2
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
        QMAKE_CXXFLAGS_RELEASE += -mfpmath=sse;
        # Others
        QMAKE_CXXFLAGS_RELEASE += -ftracer
    }
}

# Platform Specific Configuration.
win32: {
    # Application icon.
    RC_FILE += resource/platform/windows/resource.rc #\
               #resource/icon/mu.ico
    #ICON = resource/icon/mu.ico
    msvc: {
        # When using MSVC, it means it could use Qt WebEngine.
        CONFIG += webengine-backend
    }
    gcc: {
        # On Windows, we cannot use WebEngine with GCC. We should compile Mail
        # with GCC using Qt Webkit. The version of Qt not later than Qt 5.5.
        # Now, Qt has abandon the Qt Webkit, using Qt WebEngine instead. This
        # module is only used for older OS.
        CONFIG += webkit-backend
    }
}

macx: {
    # Compiled under Mac OS X, we could use the latest Qt WebEngine.
    CONFIG += webengine-backend
}

linux: {
    # We could check the version of Qt. If the Qt is later than Qt 5.5, then we
    # could use Qt WebEngine.
    lessThan(QT_MINOR_VERSION, 6) {
        # Use Qt Webkit as the backend.
        CONFIG += webkit-backend
    }
    greaterThan(QT_MINOR_VERSION, 5) {
        # Use Qt WebEngine as the backend.
        CONFIG += webengine-backend
    }
}

# Web Backend Specific Configuration
webkit-backend: {
    # Check whether there's a backend enabled already
    contains(DEFINES, BACKEND_ENABLED){
        error("You can't enable more than one backend at the same time.")
    }
    # Define the Webkit backend macro.
    DEFINES += BACKEND_WEBKIT BACKEND_ENABLED
    # Add Qt modules.
    QT += webkitwidgets
    # Add header files.
    HEADERS += \
        plugins/kmmailcomponent/plugins/kmmailcomponentwebkit/kmmailcomponentwebkit.h
    # Add source files.
    SOURCES += \
        plugins/kmmailcomponent/plugins/kmmailcomponentwebkit/kmmailcomponentwebkit.cpp
}

webengine-backend: {
    # Check whether there's a backend enabled already
    contains(DEFINES, BACKEND_ENABLED){
        error("You can't enable more than one backend at the same time.")
    }
    # Define the Webkit backend macro.
    DEFINES += BACKEND_WEBENGINE BACKEND_ENABLED
    # Add Qt modules
    QT += webenginewidgets
    # Add header files.
    HEADERS += \
        plugins/kmmailcomponent/plugins/kmmailcomponentwebengine/kmmailcomponentwebengine.h
    # Add source files.
    SOURCES += \
        plugins/kmmailcomponent/plugins/kmmailcomponentwebengine/kmmailcomponentwebengine.cpp
}

HEADERS += \
    sdk/knsingletonapplication.h \
    sdk/kmmainwindow.h \
    core/kmpluginmanager.h \
    sdk/knversion.h \
    sdk/kmglobal.h \
    sdk/kmmainwindowcontainer.h \
    sdk/kmsmtpclient.h \
    sdk/knconnectionhandler.h \
    sdk/kmpop3client.h \
    sdk/kmabstractreceiverclient.h \
    sdk/kmabstractmailclient.h \
    sdk/kmimapclient.h \
    sdk/kmabstractsenderclient.h \
    sdk/kmcontactlistmodel.h \
    sdk/kmutil.h \
    sdk/knconfigure.h \
    sdk/knconfiguremanager.h \
    sdk/knlocalemanager.h \
    sdk/knthememanager.h \
    sdk/knfontmanager.h \
    sdk/crypto/kmcryptoaes.h \
    sdk/crypto/kmcryptoaesprivate.h \
    sdk/kmmailreceivermanager.h \
    sdk/mime/kmmimebase.h \
    sdk/mime/kmquotedprintable.h \
    sdk/mime/kmmimetext.h \
    sdk/mime/kmmimehtml.h \
    sdk/mime/kmmimefile.h \
    sdk/mime/kmmimeinlinefile.h \
    sdk/mime/kmmimeattachment.h \
    sdk/mime/kmmimemultipart.h \
    sdk/mime/kmmailbody.h \
    sdk/mime/kmmimeparser.h \
    sdk/kmmainwindowleftcontainer.h \
    sdk/kmtitlebarbase.h \
    plugins/kmtitlebar/kmtitlebar.h \
    sdk/kmmaillistview.h \
    sdk/kmmaillistmodel.h \
    sdk/kmmailutil.h \
    sdk/kmleftbarbase.h \
    plugins/kmleftbar/kmleftbar.h \
    sdk/knsideshadowwidget.h \
    sdk/kmmaillistviewdelegate.h \
    sdk/kmmailcomponentbase.h \
    plugins/kmmailcomponent/kmmailcomponent.h \
    plugins/kmmailcomponent/kmmailcomponenttitlebar.h \
    sdk/sao/knsaostyle.h \
    sdk/kmmailcontactbutton.h \
    sdk/kmmailcontactlabelbutton.h \
    sdk/kmmailcontactappendbutton.h \
    sdk/kmmailcontactaddbutton.h \
    sdk/kmcoverlayer.h \
    sdk/kmunibarbase.h \
    plugins/kmunibar/kmunibar.h \
    sdk/kmtitlebarbutton.h \
    sdk/kmtitlebarcombo.h \
    sdk/kmpreferencebase.h \
    plugins/kmpreference/kmpreference.h \
    sdk/kmextendbutton.h \
    plugins/kmunibar/kmunibarcontent.h \
    plugins/kmunibar/kmunibarbutton.h \
    plugins/kmunibar/kmunibaraccountlist.h \
    sdk/knlabelbutton.h \
    plugins/kmunibar/kmunibarlabelbutton.h \
    plugins/kmmailcomponent/kmmailcomponentcontactarea.h \
    plugins/kmmailcomponent/kmmailcomponentcontactlist.h \
    plugins/kmmailcomponent/sdk/kmmailcomponentcontentbase.h \
    sdk/mailaccount/kmmailaccount.h \
    sdk/composepanel/kmcomposepanel.h \
    sdk/composepanel/kmcomposelineedit.h \
    sdk/composepanel/kmcomposeeditor.h \
    sdk/composepanel/kmcomposebutton.h \
    sdk/mailaccount/kmmailaccountmanager.h \
    sdk/composepanel/kmcomposetextedit.h

SOURCES += \
    sdk/knsingletonapplication.cpp \
    main.cpp \
    sdk/kmmainwindow.cpp \
    core/kmpluginmanager.cpp \
    sdk/kmglobal.cpp \
    sdk/kmmainwindowcontainer.cpp \
    sdk/kmsmtpclient.cpp \
    sdk/knconnectionhandler.cpp \
    sdk/kmpop3client.cpp \
    sdk/kmabstractmailclient.cpp \
    sdk/kmimapclient.cpp \
    sdk/kmcontactlistmodel.cpp \
    sdk/kmutil.cpp \
    sdk/knconfigure.cpp \
    sdk/knconfiguremanager.cpp \
    sdk/knlocalemanager.cpp \
    sdk/knthememanager.cpp \
    sdk/knfontmanager.cpp \
    sdk/crypto/kmcryptoaes.cpp \
    sdk/kmmailreceivermanager.cpp \
    sdk/mime/kmmimebase.cpp \
    sdk/mime/kmquotedprintable.cpp \
    sdk/mime/kmmimefile.cpp \
    sdk/mime/kmmimemultipart.cpp \
    sdk/mime/kmmailbody.cpp \
    sdk/mime/kmmimeparser.cpp \
    sdk/kmmainwindowleftcontainer.cpp \
    plugins/kmtitlebar/kmtitlebar.cpp \
    sdk/kmmaillistview.cpp \
    sdk/kmmaillistmodel.cpp \
    plugins/kmleftbar/kmleftbar.cpp \
    sdk/knsideshadowwidget.cpp \
    sdk/kmmaillistviewdelegate.cpp \
    plugins/kmmailcomponent/kmmailcomponent.cpp \
    plugins/kmmailcomponent/kmmailcomponenttitlebar.cpp \
    sdk/sao/knsaostyle.cpp \
    sdk/kmmailcontactbutton.cpp \
    sdk/kmmailcontactlabelbutton.cpp \
    sdk/kmmailcontactappendbutton.cpp \
    sdk/kmmailcontactaddbutton.cpp \
    sdk/kmcoverlayer.cpp \
    plugins/kmunibar/kmunibar.cpp \
    sdk/kmtitlebarbutton.cpp \
    sdk/kmtitlebarcombo.cpp \
    plugins/kmpreference/kmpreference.cpp \
    sdk/kmextendbutton.cpp \
    plugins/kmunibar/kmunibarcontent.cpp \
    plugins/kmunibar/kmunibarbutton.cpp \
    plugins/kmunibar/kmunibaraccountlist.cpp \
    sdk/knlabelbutton.cpp \
    plugins/kmunibar/kmunibarlabelbutton.cpp \
    plugins/kmmailcomponent/kmmailcomponentcontactarea.cpp \
    plugins/kmmailcomponent/kmmailcomponentcontactlist.cpp \
    plugins/kmmailcomponent/sdk/kmmailcomponentcontentbase.cpp \
    sdk/mailaccount/kmmailaccount.cpp \
    sdk/composepanel/kmcomposepanel.cpp \
    sdk/composepanel/kmcomposelineedit.cpp \
    sdk/composepanel/kmcomposeeditor.cpp \
    sdk/composepanel/kmcomposebutton.cpp \
    sdk/mailaccount/kmmailaccountmanager.cpp \
    sdk/composepanel/kmcomposetextedit.cpp

RESOURCES += \
    resource/res.qrc
