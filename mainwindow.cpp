#include "mainwindow.h"
#include "qhotkey.h"
#include "qtmaterialtoggle.h"
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
                                          hotkey(new QHotkey(this)), settings("WY", "CopyPlusPlus", this)
{
    ui->setupUi(this);

    setFixedSize(420, 360);
    setFocusPolicy(Qt::ClickFocus);

    ui->toggle1->setName("自动合并");
    ui->toggle2->setName("快捷键合并");

    connect(ui->toggle2->m_toggle, &QtMaterialToggle::toggled, this, &MainWindow::enableHotkey);
    connect(ui->keySequenceEdit, &QKeySequenceEdit::editingFinished, this, &MainWindow::keySequenceEditFinished);
    connect(hotkey, &QHotkey::activated, this, &MainWindow::hotkeyActivated);

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

    if (settings.value("toggle1", false).toBool())
    {
        ui->toggle1->setChecked(true);
        connect(QGuiApplication::clipboard(), &QClipboard::changed, this, &MainWindow::afterChanged);
    }

    if (settings.value("toggle2", false).toBool())
    {
        ui->toggle2->setChecked(true);
    }
}

void MainWindow::enableHotkey(bool status)
{
    ui->keySequenceEdit->setEnabled(status);

    if (status)
    {
        qDebug() << "Hotkey enabled.";
        registerHotkey(ui->keySequenceEdit->keySequence());
    }
    else
    {
        qDebug() << "Hotkey disabled.";
        hotkey->resetShortcut();
    }
}

void MainWindow::keySequenceEditFinished()
{
    settings.setValue("shortcut", ui->keySequenceEdit->keySequence().toString());
    registerHotkey(ui->keySequenceEdit->keySequence());
}

// Register hotkey
void MainWindow::registerHotkey(const QKeySequence &keySequence)
{
    hotkey->setShortcut(keySequence, true);
    qDebug() << "Hotkey" << keySequence << "registered:" << hotkey->isRegistered();
}

void MainWindow::hotkeyActivated()
{
    qDebug() << "Hotkey activated";
    pressCtrlC();
    processClipboard();
}

void MainWindow::saveSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);
    settings.setValue("toggle1", ui->toggle1->isChecked());
    settings.setValue("toggle2", ui->toggle2->isChecked());
}

void MainWindow::processClipboard()
{
    QString s = QGuiApplication::clipboard()->text();

    qDebug() << "Before :" << s;

    s.replace("\r", "");
    s.replace("\n", "");

    QGuiApplication::clipboard()->setText(s);

    qDebug() << "After :" << QGuiApplication::clipboard()->text();
}

void MainWindow::afterChanged()
{
    qDebug() << "triggered";
    flag = !flag;
    if (flag)
    {
        processClipboard();
    }
}

void MainWindow::pressCtrlC()
{
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
