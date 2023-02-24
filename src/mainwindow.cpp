#include "mainwindow.h"
#include "language.h"
#include "qhotkey.h"
#include "qtmaterialtoggle.h"
#include "settingswindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QSettings>
#include <QThread>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winuser.h>
#endif

#ifdef Q_OS_MAC
#include <Carbon/Carbon.h>
#endif

MainWindow *MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    if (instance != nullptr) {
        qCritical() << "创建多个MainWindow instance，这里应该是个Bug";
    }
    instance = this;

    hotkey = new QHotkey(this);
    translator = new QTranslator(this);

    initUI();

    Language::initLanguage();

    updateText();

    initConnections();

    // settingsIniFile = QApplication::applicationDirPath() + "/settings.ini";
    loadSettings();
}

MainWindow::~MainWindow()
{
    instance = nullptr;

    delete ui;
}

MainWindow *MainWindow::getInstance()
{
    if (instance == nullptr) {
        qDebug() << "MainWindow instance为空，这里应该是个Bug";
    }

    return instance;
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        // 感觉没啥用
        ui->retranslateUi(this);

        // 更新文本，用于翻译
        updateText();
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::initUI()
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/icons/images/copy.png"));

    setFocusPolicy(Qt::ClickFocus);
    setFixedSize(273, 345);

    settingsWindow = nullptr;

    // 初始化 toggle
    autoToggle = new QtMaterialToggle;
    auto h = new QHBoxLayout();
    ui->toggleWidget->setLayout(h);
    h->setContentsMargins(0, 0, 0, 0);
    h->addWidget(autoToggle, Qt::AlignRight);

    // 设置按钮
    floatBtn = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("settings"), this);
    floatBtn->setMini(true);

#ifdef Q_OS_MAC
    // mac 暂不支持自动合并
    autoToggle->setEnabled(false);
    autoToggle->setToolTip(tr("Mac 暂不支持自动合并"));
#endif
}

// 更新文本，用于翻译
void MainWindow::updateText()
{
    setWindowTitle(tr("CopyPlusPlus"));

    ui->autoLable->setText(tr("自动触发"));
    ui->hotkeyLable->setText(tr("快捷键"));
    ui->keySequenceEdit->lineEdit->setPlaceholderText(tr("快捷键"));
}

void MainWindow::initConnections()
{
    connect(autoToggle, &QtMaterialToggle::toggled, this, &MainWindow::autoToggleChecked);

    connect(ui->keySequenceEdit, &MyKeySequenceEdit::myEditFinished, this, &MainWindow::registerShortcut);
    connect(ui->keySequenceEdit, &MyKeySequenceEdit::focusIn, this, [&]() { hotkey->setRegistered(false); });
    connect(ui->keySequenceEdit, &MyKeySequenceEdit::focusOut, this, [&]() { hotkey->setRegistered(true); });

    connect(hotkey, &QHotkey::activated, this, &MainWindow::shortcutTriggered);

    connect(floatBtn, &QtMaterialFloatingActionButton::clicked, this, [&]() {
        if (settingsWindow == nullptr) {
            settingsWindow = new SettingsWindow(this);

            connect(settingsWindow, &SettingsWindow::closed, this, [&]() { settingsWindow = nullptr; });
            settingsWindow->show();
        }
    });
}

void MainWindow::loadSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat);

    if (settings.value("autoToggle", false).toBool()) {
        autoToggle->setChecked(true);
    }

    registerShortcut(settings.value("shortcut", "Ctrl+Shift+C").toString());
}

void MainWindow::saveSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);

    settings.setValue("autoToggle", autoToggle->isChecked());
    settings.setValue("shortcut", hotkey->shortcut().toString());

    qDebug() << "Save shortcut: " << hotkey->shortcut().toString();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::autoToggleChecked(bool status)
{
    if (status) {
        qDebug() << "Auto enabled";
        connect(QGuiApplication::clipboard(), &QClipboard::changed, this, &MainWindow::afterChanged);
    } else {
        qDebug() << "Auto disabled";
        disconnect(QGuiApplication::clipboard(), &QClipboard::changed, this, &MainWindow::afterChanged);
    }
}

// void MainWindow::toggleShortcutChecked(bool status)
//{
//     // TODO:快捷键为空、冲突时，应该有提醒
//     QString seq = settings.value("shortcut", "Ctrl+Shift+C").toString();
//     if (status) {
//         qDebug() << "Shortcut enabled";
//         registerShortcut(seq);
//     } else {
//         qDebug() << "Shortcut disabled";
//         hotkey->resetShortcut();
//     }
// }

// Register shortcut
void MainWindow::registerShortcut(const QKeySequence &keySequence)
{
    // what's autoRegister?
    if (!hotkey->setShortcut(keySequence, true) && !keySequence.isEmpty()) {
        errorInput();
    }

    qDebug() << "Shortcut" << keySequence << "registered:" << hotkey->isRegistered();
}

void MainWindow::errorInput()
{
    hotkey->resetShortcut();

    QMessageBox msgBox;
    msgBox.setText(tr("快捷键占用，请更换快捷键。"));
    msgBox.exec();
}

void MainWindow::shortcutTriggered()
{
    // qDebug() << "Shortcut activated";

    pressCtrlC();
    processClipboard();

    // 还会再触发一次 afterChanged()
    flag = true;
}

void MainWindow::processClipboard()
{
    QThread::msleep(50);

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
#elif defined(Q_OS_WIN)
    setClipboardTextWin(s);
#endif

    qDebug() << "After:" << QGuiApplication::clipboard()->text();
}

// 目前快捷键 setClipboard 后还会触发一次
void MainWindow::afterChanged()
{
    // qDebug() << "Clipboard changed";

    // TODO: 改用 EventFilter
    flag = !flag;
    if (flag) {
        processClipboard();
    }
}

void MainWindow::pressCtrlC()
{
#ifdef Q_OS_WIN
    // QStringList keys = ui->keySequenceEdit->keySequence().toString().split("+");
    QStringList keys = settings.value("shortcut", "Ctrl+Shift+C").toString().split("+");

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
#elif defined(Q_OS_MAC)
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
