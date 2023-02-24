QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# App icon
mac: ICON = images/cpp-120.ico
else:win32: RC_ICONS = images/cpp-120.ico
else:unix {
}

# Force refresh UI
UI_DIR = $$PWD

INCLUDEPATH += 3rdparty/material 3rdparty/QHotkey src/ src/utils

# material files
SOURCES += $$files(3rdparty/material/*.cpp, true)
HEADERS += $$files(3rdparty/material/*.h, true)

# QHotKey files
SOURCES += 3rdparty/QHotKey/qhotkey.cpp
HEADERS += 3rdparty/QHotKey/qhotkey.h 3rdparty/QHotKey/qhotkey_p.h
mac: SOURCES += 3rdparty/QHotKey/qhotkey_mac.cpp
else:win32: SOURCES += 3rdparty/QHotkey/qhotkey_win.cpp
else:unix: SOURCES += 3rdparty/QHotkey/qhotkey_x11.cpp

# QHotKey Settings
mac: LIBS += -framework Carbon
else:win32: LIBS += -luser32
else:unix {
    equals(QT_MAJOR_VERSION, 6) {
        lessThan(QT_VERSION, 6.2.0) { error("Qt 6.2.0 or greater is required when using Qt6") }
    } else {
        lessThan(QT_MAJOR_VERSION, 6) { QT += x11extras }
    }
    LIBS += -lX11
}

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mykeysequenceedit.cpp \
    src/settingswindow.cpp \
    src/toggle.cpp \
    src/utils/language.cpp

HEADERS += \
    src/mainwindow.h \
    src/mykeysequenceedit.h \
    src/settingswindow.h \
    src/toggle.h \
    src/utils/language.h

FORMS += \
    src/mainwindow.ui \
    src/settingswindow.ui \
    src/toggle.ui

RESOURCES += resources.qrc

TRANSLATIONS += CopyPlusPlus-Qt_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
