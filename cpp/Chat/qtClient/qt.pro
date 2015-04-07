
TEMPLATE = app

TARGET = QtChatDemo
DESTDIR = .

CONFIG += qt warn_on

QT = core gui

equals(QT_MAJOR_VERSION, "5"){
    QT = widgets
}

HEADERS += Util.h \
    Coordinator.h \
    LoginView.h \
    ChatView.h \
    MainView.h

SOURCES += Util.cpp \
    Chat.cpp \
    ChatSession.cpp \
    LoginView.cpp \
    ChatView.cpp \
    MainView.cpp \
    Coordinator.cpp


OPTIMIZE = $$(OPTIMIZE)

equals(OPTIMIZE,"yes"){
    CONFIG += release
}
else{
    CONFIG += debug
}

ICE_HOME = $$(ICE_HOME)

INCLUDEPATH   += .

!equals(ICE_HOME,"/usr"){
    INCLUDEPATH   += $$quote($$ICE_HOME/include)
}

win32{
    ICON = resources/icons/logo.png
    DEFINES += WIN32_LEAN_AND_MEAN
    RC_FILE = client.rc
    CONFIG(debug,debug|release){
        LIBS += "IceUtild.lib" \
                "Iced.lib" \
                "IceSSLd.lib" \
                "Glacier2d.lib"
    }
    else{
        LIBS += "IceUtil.lib" \
                "Ice.lib" \
                "IceSSL.lib" \
                "Glacier2.lib"
    }
    ICE_x64suffix = $$(ICE_x64suffix)
    QMAKE_LIBDIR  += $$quote($$ICE_HOME/lib$$ICE_x64suffix)
}
else{
    !equals(ICE_HOME,"/usr"){
        ICE_libsubdir = $$(ICE_libsubdir)
        QMAKE_LIBDIR  += $$quote($$ICE_HOME/$$ICE_libsubdir)
    }
}

mac{
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.8
    QMAKE_INFO_PLIST = $$TOPDIR/config/Info.plist
    QMAKE_LFLAGS    += -Wl,-rpath,@executable_path/../Frameworks/
    ICON = resources/icons/logo.icns
}

unix:!mac{
    ICON = resources/icons/logo.png
}

unix{
    LIBS += -lIceUtil -lIce -lIceSSL -lGlacier2
}

RESOURCES += qt.qrc
