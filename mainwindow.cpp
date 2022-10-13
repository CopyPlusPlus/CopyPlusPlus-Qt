#include "mainwindow.h"
#include "qhotkey.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QSettings>
#include <QThread>

#ifdef Q_OS_MAC
#include <Carbon/Carbon.h>
#endif

#ifdef Q_OS_MAC
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    settings("WY", "CopyPlusPlus", this) {
    ui->setupUi(this);

    setFixedSize(420, 360);
    ui->toggle1->setName("自动合并");
    ui->toggle2->setName("快捷键合并");
    ui->toggle1->connectToggled(&);

#ifdef Q_OS_MAC
    // mac 暂不支持自动合并
    ui->toggle1->setEnabled(false);
#endif
    // settingsIniFile = QApplication::applicationDirPath() + "/settings.ini";
    loadSettings();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::ininGui() {}

void MainWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    event->accept();
}

void MainWindow::loadSettings() {
    // QSettings settings(settingsIniFile, QSettings::IniFormat);

    if (settings.value("toggle1", false).toBool()) {
        ui->toggle1->setChecked(true);

        connect(QGuiApplication::clipboard(), &QClipboard::changed, this, &MainWindow::afterChanged);
    }

    if (settings.value("toggle2", false).toBool()) {
        ui->toggle2->setChecked(true);

        //registerHotkey(true);
    }
}

// Register hotkey
void MainWindow::registerHotkey(bool status) {
    if (status) {
        hotkey = new QHotkey(QKeySequence(settings.value("shortcut", "Ctrl+Shift+C").toString()), true, qApp);
        qDebug() << "Is segistered:" << hotkey->isRegistered();

        connect(hotkey, &QHotkey::activated, qApp, [&]() {
            qDebug() << "Hotkey activated";
            pressCtrlC();
            processClipboard();
        });
    } else {
        hotkey->resetShortcut();
    }

}

void MainWindow::saveSettings() {
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);
    settings.setValue("toggle1", ui->toggle1->isChecked());
    settings.setValue("toggle2", ui->toggle2->isChecked());
}

void MainWindow::processClipboard() {
    QString s = QGuiApplication::clipboard()->text();

    qDebug() << "Before :" << s;

    s.replace("\r", "");
    s.replace("\n", "");

    QGuiApplication::clipboard()->setText(s);

    qDebug() << "After :" << QGuiApplication::clipboard()->text();
}

bool flag = false;
void MainWindow::afterChanged() {
    qDebug() << "triggered";
    flag = !flag;
    if (flag) {
        processClipboard();
    }
}

void MainWindow::pressCtrlC() {
#ifdef Q_OS_MAC
    CGKeyCode inputKeyCode = kVK_ANSI_C;
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventRef saveCommandDown = CGEventCreateKeyboardEvent(source, inputKeyCode, true);
    CGEventSetFlags(saveCommandDown, kCGEventFlagMaskCommand);
    CGEventRef saveCommandUp = CGEventCreateKeyboardEvent(source, inputKeyCode, false);

    CGEventPost(kCGAnnotatedSessionEventTap, saveCommandDown);
    CGEventPost(kCGAnnotatedSessionEventTap, saveCommandUp);

    CFRelease(saveCommandUp);
    CFRelease(saveCommandDown);
    CFRelease(source);
#endif
}
