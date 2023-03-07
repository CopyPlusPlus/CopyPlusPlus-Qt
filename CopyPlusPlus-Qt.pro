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
UI_DIR = $$PWD/src/ui

INCLUDEPATH += 3rdparty/material 3rdparty/QHotkey src/ src/ui src/components

SOURCES += \
    src/main.cpp \
    src/ui/mainwindow.cpp \
    src/ui/settingswindow.cpp \
    src/components/mykeysequenceedit.cpp \
    src/components/toggle.cpp \
    src/utils/language.cpp \
    src/utils/textprocessor.cpp

HEADERS += \
    src/ui/mainwindow.h \
    src/ui/settingswindow.h \
    src/components/mykeysequenceedit.h \
    src/components/toggle.h \
    src/utils/language.h \
    src/utils/textprocessor.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/settingswindow.ui \
    src/components/toggle.ui

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

RESOURCES += resources.qrc

TRANSLATIONS += $$files(translations/*.ts, true)

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
