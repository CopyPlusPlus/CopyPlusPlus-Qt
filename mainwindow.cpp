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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    settings("WY", "CopyPlusPlus", this) {
    ui->setupUi(this);

    setFixedSize(420, 360);
    ui->toggle1->setName("自动合并");
    ui->toggle2->setName("快捷键合并");

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

        // Register hotkey
        hotkey = new QHotkey(QKeySequence("Ctrl+Shift+C"), true, qApp);
        qDebug() << "Is segistered:" << hotkey->isRegistered();

        connect(hotkey, &QHotkey::activated, qApp, [&]() {
            qDebug() << "Hotkey activated";
            pressCtrlC();
        });
    }
}

void MainWindow::saveSettings() {
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);
    settings.setValue("toggle1", ui->toggle1->isChecked());
    settings.setValue("toggle2", ui->toggle2->isChecked());
}

void MainWindow::process() {
    qDebug() << "Clipboard changed";

    QString s = QGuiApplication::clipboard()->text();

    qDebug() << "Before :" << s;

    s.replace("r", "a");

    QGuiApplication::clipboard()->setText(s);

    qDebug() << "After :" << QGuiApplication::clipboard()->text();
}

bool flag = false;
void MainWindow::afterChanged() {
    qDebug() << "triggered";
    flag = !flag;
    if (flag) {
        process();
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
