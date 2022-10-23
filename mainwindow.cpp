#include "mainwindow.h"
#include "mykeysequenceedit.h"
#include "qhotkey.h"
#include "qtmaterialtoggle.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QMimeData>
#include <QSettings>
#include <QThread>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winuser.h>
#endif

#ifdef Q_OS_MAC
#include <Carbon/Carbon.h>
#endif

#ifdef Q_OS_MAC
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
                                          hotkey(new QHotkey(this)), settings("WY", "CopyPlusPlus", this)
{
    ui->setupUi(this);

    setFixedSize(420, 360);
    setFocusPolicy(Qt::ClickFocus);

    ui->toggle1->setName("自动合并");
    ui->toggle2->setName("快捷键合并");

    connect(ui->toggle1->m_toggle, &QtMaterialToggle::toggled, this, &MainWindow::toggleAutoEnabled);
    connect(ui->toggle2->m_toggle, &QtMaterialToggle::toggled, this, &MainWindow::setShortcutEnabled);

    connect(ui->keySequenceEdit, &myKeySequenceEdit::focusIn, this, [&]() { qDebug() << "Shortcut reseted"; hotkey->resetShortcut(); });

    // editingFinished: 仅在输入结束时触发, setKeySequence 不触发
    // keySequenceChanged: 输入结束以及 setKeySequence 时触发
    // connect(ui->keySequenceEdit, &QKeySequenceEdit::editingFinished, this, &MainWindow::truncateShortcut);
    connect(ui->keySequenceEdit, &QKeySequenceEdit::editingFinished, this, &MainWindow::keySequenceEditFinished);

    connect(hotkey, &QHotkey::activated, this, &MainWindow::shortcutTriggered);

#ifdef Q_OS_MAC
    // mac 暂不支持自动合并
    ui->toggle1->setEnabled(false);
#endif

    // settingsIniFile = QApplication::applicationDirPath() + "/settings.ini";
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ininGui() {}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::loadSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat);

    ui->keySequenceEdit->setKeySequence(QKeySequence(settings.value("shortcut", "Ctrl+Shift+C").toString()));

    if (settings.value("toggle1", false).toBool()) {
        ui->toggle1->setChecked(true);
    }

    if (settings.value("toggle2", false).toBool()) {
        ui->toggle2->setChecked(true);
    }
}

void MainWindow::toggleAutoEnabled()
{
    if (ui->toggle1->isChecked()) {
        qDebug() << "Auto enabled";
        connect(QGuiApplication::clipboard(), &QClipboard::changed, this, &MainWindow::afterChanged);
    } else {
        qDebug() << "Auto disabled";
        disconnect(QGuiApplication::clipboard(), &QClipboard::changed, this, &MainWindow::afterChanged);
    }
}

void MainWindow::setShortcutEnabled(bool status)
{
    ui->keySequenceEdit->setEnabled(status);

    if (status) {
        qDebug() << "Shortcut enabled";
        registerShortcut(ui->keySequenceEdit->keySequence());
    } else {
        qDebug() << "Shortcut disabled";
        hotkey->resetShortcut();
    }
}

void MainWindow::keySequenceEditFinished()
{
    ui->keySequenceEdit->clearFocus();

    settings.setValue("shortcut", ui->keySequenceEdit->keySequence().toString());
    registerShortcut(ui->keySequenceEdit->keySequence());
}

// Make keySequenceEdit only show one shortcut
// void MainWindow::truncateShortcut()
// {
//     ui->keySequenceEdit->clearFocus();
//     ui->keySequenceEdit->setKeySequence(ui->keySequenceEdit->keySequence()[0]);
// }

// Register shortcut
void MainWindow::registerShortcut(const QKeySequence &keySequence)
{
    hotkey->setShortcut(keySequence, true);
    qDebug() << "Shortcut" << keySequence << "registered:" << hotkey->isRegistered();
}

void MainWindow::shortcutTriggered()
{
    qDebug() << "Shortcut activated";

    pressCtrlC();
    processClipboard();

    // 还会再触发一次 afterChanged()
    flag = true;
}

void MainWindow::saveSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);
    settings.setValue("toggle1", ui->toggle1->isChecked());
    settings.setValue("toggle2", ui->toggle2->isChecked());
}

void MainWindow::processClipboard()
{
    Sleep(50);

    // 防止截图等仅复制图片时处理导致无法复制
    if (!QGuiApplication::clipboard()->mimeData()->hasText()) {
        return;
    }

    QString s = QGuiApplication::clipboard()->text();

    qDebug() << "Before:" << s;

    s.replace("\r", "");
    s.replace("\n", "");

#ifdef Q_OS_MAC
    QGuiApplication::clipboard()->setText(s);
#endif

#ifdef Q_OS_WIN
    setClipboardTextWin(s);
#endif

    qDebug() << "After:" << QGuiApplication::clipboard()->text();
}

// 目前快捷键 setClipboard 后还会触发一次
void MainWindow::afterChanged()
{
    qDebug() << "Clipboard changed";

    // TODO: 改用 EventFilter
    flag = !flag;
    if (flag) {
        processClipboard();
    }
}

void MainWindow::pressCtrlC()
{
#ifdef Q_OS_WIN
    QStringList keys = ui->keySequenceEdit->keySequence().toString().split("+");

    const int n = keys.size();
    INPUT inputs[n + 4];
    ZeroMemory(inputs, sizeof(inputs));

    // 释放 Modifier
    for (int i = 0; i < n - 1; ++i) {
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;

        switch (keys[i].toStdString()[0]) {
        case 'C':
            inputs[i].ki.wVk = VK_LCONTROL;
            break;
        case 'S':
            inputs[i].ki.wVk = VK_LSHIFT;
            break;
        case 'M':
            inputs[i].ki.wVk = VK_LWIN;
            break;
        case 'A':
            inputs[i].ki.wVk = VK_LMENU;
            break;
        }
    }
    // 释放快捷键
    inputs[n - 1].type = INPUT_KEYBOARD;
    inputs[n - 1].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[n - 1].ki.wVk = VkKeyScanA(keys[n - 1].toStdString()[0]);

    // 按下 CTRL
    inputs[n].type = INPUT_KEYBOARD;
    inputs[n].ki.wVk = VK_LCONTROL;
    inputs[n].ki.dwFlags = 0;
    // 按下 C
    inputs[n + 1].type = INPUT_KEYBOARD;
    inputs[n + 1].ki.wVk = 0x43; // C
    inputs[n + 1].ki.dwFlags = 0;
    // 释放 CTRL
    inputs[n + 2].type = INPUT_KEYBOARD;
    inputs[n + 2].ki.wVk = VK_LCONTROL;
    inputs[n + 2].ki.dwFlags = KEYEVENTF_KEYUP;
    // 释放 C
    inputs[n + 3].type = INPUT_KEYBOARD;
    inputs[n + 3].ki.wVk = 0x43; // C
    inputs[n + 3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs)) {
        qDebug() << "SendInput failed:" << HRESULT_FROM_WIN32(GetLastError());
    } else {
        qDebug() << "SendInput succeed";
    }
#endif

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

#ifdef Q_OS_WIN
void MainWindow::setClipboardTextWin(QString _text)
{
    QByteArray ba = _text.toLocal8Bit();
    const char *text = ba.data();
    const size_t len = strlen(text) + 1;

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);

    while (!OpenClipboard(0)) {
        qDebug() << "Cannot open clipboard";
        Sleep(50);
    }

    while (!EmptyClipboard()) {
        qDebug() << "Cannot empty clipboard";
        Sleep(50);
    }

    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}
#endif
