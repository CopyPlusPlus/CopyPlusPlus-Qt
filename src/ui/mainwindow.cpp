#include "mainwindow.h"
#include "qhotkey.h"
#include "settingswindow.h"
#include "utils/language.h"
#include "utils/textprocessor.h"

#include "ui_mainwindow.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QtWidgets>

MainWindow *MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Singleton pattern
    if (instance != nullptr) {
        qCritical() << "创建多个MainWindow instance, 这里应该是个Bug";
    }
    instance = this;

    hotkey = new QHotkey(this);
    translator = new QTranslator(this);

    ui->setupUi(this);

    settingsBtn = ui->bottomWidget->findChild<QtMaterialFlatButton *>("settingsBtn");
    aboutBtn = ui->bottomWidget->findChild<QtMaterialFlatButton *>("aboutBtn");
    settingsWindow = nullptr;

    initUI();

    Language::initLanguage();

    updateText();

    initConnections();

    // settingsIniFile = QApplication::applicationDirPath() + "/settings.ini";
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::getInstance()
{
    if (instance == nullptr) {
        qDebug() << "MainWindow instance为空, 这里应该是个Bug";
    }

    return instance;
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);

        // 更新文本, 用于翻译
        updateText();
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::initUI()
{
    ui->autoToggle->setText(tr("Auto Mode"));
    ui->hotkeyToggle->setText(tr("Hotkey Mode"));

    setFlatBtnStyle(aboutBtn);
    setFlatBtnStyle(settingsBtn);

    setFixedSize(size()); // 放到最后，更新并固定窗口大小

#ifdef Q_OS_MAC
    // mac 暂不支持自动合并
    // autoToggle->setEnabled(false);
    // autoToggle->setToolTip(tr("Mac 暂不支持自动合并"));
#endif
}

// 更新文本, 用于翻译
void MainWindow::updateText()
{
    setWindowTitle(tr("CopyPlusPlus"));

    ui->autoToggle->setText(tr("Auto Mode"));
    ui->hotkeyToggle->setText(tr("Hotkey Mode"));

    // ui->keySequenceEdit->lineEdit->setPlaceholderText(tr("快捷键"));
}

void MainWindow::setFlatBtnStyle(QtMaterialFlatButton *fBtn)
{
    // fBtn->setIcon(QIcon(":/icons/settings"));
    fBtn->setRole(Material::Primary);
    fBtn->setBackgroundMode(Qt::OpaqueMode);
    fBtn->setFont(QFont("Microsoft YaHei"));
}

void MainWindow::initConnections()
{
    connect(ui->autoToggle, &Toggle::toggled, this, &MainWindow::autoToggleChecked);

    // connect(ui->keySequenceEdit, &MyKeySequenceEdit::myEditFinished, this, &MainWindow::registerShortcut);
    // connect(ui->keySequenceEdit, &MyKeySequenceEdit::focusIn, this, [&]() { hotkey->setRegistered(false); });
    // connect(ui->keySequenceEdit, &MyKeySequenceEdit::focusOut, this, [&]() { hotkey->setRegistered(true); });

    connect(hotkey, &QHotkey::activated, this, &MainWindow::shortcutTriggered);

    connect(settingsBtn, &QtMaterialFlatButton::clicked, this, [&]() {
        if (settingsWindow == nullptr) {
            settingsWindow = new SettingsWindow(this);

            connect(settingsWindow, &SettingsWindow::closed, this, [&]() { settingsWindow = nullptr; });

            settingsWindow->show();
            settingsWindow->raise();
            settingsWindow->activateWindow();
        }
    });
}

void MainWindow::loadSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat);

    if (settings.value("autoToggle", false).toBool()) {
        ui->autoToggle->setChecked(true);
    }
    if (settings.value("hotkeyToggle", false).toBool()) {
        ui->hotkeyToggle->setChecked(true);
    }

    registerShortcut(settings.value("shortcut", "Ctrl+Shift+C").toString());
}

void MainWindow::saveSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);

    settings.setValue("autoToggle", ui->autoToggle->isChecked());
    settings.setValue("hotkeyToggle", ui->hotkeyToggle->isChecked());

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

void MainWindow::hotkeyToggleChecked(bool status)
{
    // TODO:快捷键为空、冲突时, 应该有提醒
    QString seq = settings.value("shortcut", "Ctrl+Shift+C").toString();
    if (status) {
        qDebug() << "Shortcut enabled";
        registerShortcut(seq);
    } else {
        qDebug() << "Shortcut disabled";
        hotkey->resetShortcut();
    }
}

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
    msgBox.setText(tr("Hotkey occupied, change it."));
    msgBox.exec();
}

void MainWindow::shortcutTriggered()
{
    // qDebug() << "Shortcut activated";

    TextProcessor::pressCtrlC();
    TextProcessor::processClipboard();

    // 还会再触发一次 afterChanged()
    flag = true;
}

// 目前快捷键 setClipboard 后还会触发一次
void MainWindow::afterChanged()
{
    // qDebug() << "Clipboard changed";

    // TODO: 改用 EventFilter
    flag = !flag;
    if (flag) {
        TextProcessor::processClipboard();
    }
}
