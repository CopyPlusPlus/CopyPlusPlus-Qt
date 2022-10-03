QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    3rdparty/QHotkey/qhotkey.cpp \
    main.cpp \
    mainwindow.cpp \
    material/lib/qtmaterialcheckable.cpp \
    material/lib/qtmaterialcheckable_internal.cpp \
    material/lib/qtmaterialoverlaywidget.cpp \
    material/lib/qtmaterialripple.cpp \
    material/lib/qtmaterialrippleoverlay.cpp \
    material/lib/qtmaterialstatetransition.cpp \
    material/lib/qtmaterialstyle.cpp \
    material/lib/qtmaterialtheme.cpp \
    material/qtmaterialtoggle.cpp \
    material/qtmaterialtoggle_internal.cpp \
    processtext.cpp \
    toggle.cpp

HEADERS += \
    3rdparty/QHotkey/qhotkey.h \
    3rdparty/QHotkey/qhotkey_p.h \
    mainwindow.h \
    material/lib/qtmaterialcheckable.h \
    material/lib/qtmaterialcheckable_internal.h \
    material/lib/qtmaterialcheckable_p.h \
    material/lib/qtmaterialoverlaywidget.h \
    material/lib/qtmaterialripple.h \
    material/lib/qtmaterialrippleoverlay.h \
    material/lib/qtmaterialstatetransition.h \
    material/lib/qtmaterialstatetransitionevent.h \
    material/lib/qtmaterialstyle.h \
    material/lib/qtmaterialstyle_p.h \
    material/lib/qtmaterialtheme.h \
    material/lib/qtmaterialtheme_p.h \
    material/qtmaterialtoggle.h \
    material/qtmaterialtoggle_internal.h \
    material/qtmaterialtoggle_p.h \
    processtext.h \
    toggle.h

FORMS += \
    mainwindow.ui \
    toggle.ui

mac: SOURCES += 3rdparty/QHotKey/qhotkey_mac.cpp
else:win32: SOURCES += 3rdparty/QHotkey/qhotkey_win.cpp
else:unix: SOURCES += 3rdparty/QHotkey/qhotkey_x11.cpp

INCLUDEPATH += material 3rdparty\QHotkey

RESOURCES += resources.qrc

TRANSLATIONS += CopyPlusPlus-Qt_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
