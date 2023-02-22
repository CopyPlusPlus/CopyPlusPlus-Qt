QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += material 3rdparty/QHotkey

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

# App icon
mac: ICON = images/cpp.ico
else:win32: RC_ICONS = images/copy.ico
else:unix {
}


# Force refresh UI
UI_DIR = $$PWD

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
    material/qtmaterialappbar.cpp \
    material/qtmaterialautocomplete.cpp \
    material/qtmaterialautocomplete_internal.cpp \
    material/qtmaterialavatar.cpp \
    material/qtmaterialbadge.cpp \
    material/qtmaterialcheckbox.cpp \
    material/qtmaterialcircularprogress.cpp \
    material/qtmaterialcircularprogress_internal.cpp \
    material/qtmaterialdialog.cpp \
    material/qtmaterialdialog_internal.cpp \
    material/qtmaterialdrawer.cpp \
    material/qtmaterialdrawer_internal.cpp \
    material/qtmaterialfab.cpp \
    material/qtmaterialflatbutton.cpp \
    material/qtmaterialflatbutton_internal.cpp \
    material/qtmaterialiconbutton.cpp \
    material/qtmateriallist.cpp \
    material/qtmateriallistitem.cpp \
    material/qtmaterialmenu.cpp \
    material/qtmaterialmenu_internal.cpp \
    material/qtmaterialpaper.cpp \
    material/qtmaterialprogress.cpp \
    material/qtmaterialprogress_internal.cpp \
    material/qtmaterialradiobutton.cpp \
    material/qtmaterialraisedbutton.cpp \
    material/qtmaterialscrollbar.cpp \
    material/qtmaterialscrollbar_internal.cpp \
    material/qtmaterialslider.cpp \
    material/qtmaterialslider_internal.cpp \
    material/qtmaterialsnackbar.cpp \
    material/qtmaterialsnackbar_internal.cpp \
    material/qtmaterialtable.cpp \
    material/qtmaterialtabs.cpp \
    material/qtmaterialtabs_internal.cpp \
    material/qtmaterialtextfield.cpp \
    material/qtmaterialtextfield_internal.cpp \
    material/qtmaterialtoggle.cpp \
    material/qtmaterialtoggle_internal.cpp \
    mykeysequenceedit.cpp \
    settingswindow.cpp \
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
    material/qtmaterialappbar.h \
    material/qtmaterialappbar_p.h \
    material/qtmaterialautocomplete.h \
    material/qtmaterialautocomplete_internal.h \
    material/qtmaterialautocomplete_p.h \
    material/qtmaterialavatar.h \
    material/qtmaterialavatar_p.h \
    material/qtmaterialbadge.h \
    material/qtmaterialbadge_p.h \
    material/qtmaterialcheckbox.h \
    material/qtmaterialcheckbox_p.h \
    material/qtmaterialcircularprogress.h \
    material/qtmaterialcircularprogress_internal.h \
    material/qtmaterialcircularprogress_p.h \
    material/qtmaterialdialog.h \
    material/qtmaterialdialog_internal.h \
    material/qtmaterialdialog_p.h \
    material/qtmaterialdrawer.h \
    material/qtmaterialdrawer_internal.h \
    material/qtmaterialdrawer_p.h \
    material/qtmaterialfab.h \
    material/qtmaterialfab_p.h \
    material/qtmaterialflatbutton.h \
    material/qtmaterialflatbutton_internal.h \
    material/qtmaterialflatbutton_p.h \
    material/qtmaterialiconbutton.h \
    material/qtmaterialiconbutton_p.h \
    material/qtmateriallist.h \
    material/qtmateriallist_p.h \
    material/qtmateriallistitem.h \
    material/qtmateriallistitem_p.h \
    material/qtmaterialmenu.h \
    material/qtmaterialmenu_internal.h \
    material/qtmaterialmenu_p.h \
    material/qtmaterialpaper.h \
    material/qtmaterialpaper_p.h \
    material/qtmaterialprogress.h \
    material/qtmaterialprogress_internal.h \
    material/qtmaterialprogress_p.h \
    material/qtmaterialradiobutton.h \
    material/qtmaterialradiobutton_p.h \
    material/qtmaterialraisedbutton.h \
    material/qtmaterialraisedbutton_p.h \
    material/qtmaterialscrollbar.h \
    material/qtmaterialscrollbar_internal.h \
    material/qtmaterialscrollbar_p.h \
    material/qtmaterialslider.h \
    material/qtmaterialslider_internal.h \
    material/qtmaterialslider_p.h \
    material/qtmaterialsnackbar.h \
    material/qtmaterialsnackbar_internal.h \
    material/qtmaterialsnackbar_p.h \
    material/qtmaterialtable.h \
    material/qtmaterialtable_p.h \
    material/qtmaterialtabs.h \
    material/qtmaterialtabs_internal.h \
    material/qtmaterialtabs_p.h \
    material/qtmaterialtextfield.h \
    material/qtmaterialtextfield_internal.h \
    material/qtmaterialtextfield_p.h \
    material/qtmaterialtoggle.h \
    material/qtmaterialtoggle_internal.h \
    material/qtmaterialtoggle_p.h \
    mykeysequenceedit.h \
    settingswindow.h \
    toggle.h

FORMS += \
    mainwindow.ui \
    settingswindow.ui \
    toggle.ui

mac: SOURCES += 3rdparty/QHotKey/qhotkey_mac.cpp
else:win32: SOURCES += 3rdparty/QHotkey/qhotkey_win.cpp
else:unix: SOURCES += 3rdparty/QHotkey/qhotkey_x11.cpp

RESOURCES += resources.qrc

TRANSLATIONS += CopyPlusPlus-Qt_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
