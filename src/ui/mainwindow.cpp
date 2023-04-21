#include "mainwindow.h"
#include "qhotkey.h"
//#include "qtmaterialtoggle.h"
#include "settingswindow.h"
#include "ui_mainwindow.h"
#include "utils/language.h"
#include "utils/textprocessor.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>

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
    setFixedSize(310, 370);

    settingsWindow = nullptr;

    // 初始化 toggle
    // autoToggle = new QtMaterialToggle;
    // auto h = new QHBoxLayout();
    // ui->toggleWidget->setLayout(h);
    // h->setContentsMargins(0, 0, 0, 0);
    // h->addWidget(autoToggle, Qt::AlignRight);

    // 设置按钮
    //floatBtn = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("settings"), this);
    //floatBtn->setMini(true);

#ifdef Q_OS_MAC
    // mac 暂不支持自动合并
    //autoToggle->setEnabled(false);
    //autoToggle->setToolTip(tr("Mac 暂不支持自动合并"));
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
    //connect(autoToggle, &QtMaterialToggle::toggled, this, &MainWindow::autoToggleChecked);

    connect(ui->keySequenceEdit, &MyKeySequenceEdit::myEditFinished, this, &MainWindow::registerShortcut);
    connect(ui->keySequenceEdit, &MyKeySequenceEdit::focusIn, this, [&]() { hotkey->setRegistered(false); });
    connect(ui->keySequenceEdit, &MyKeySequenceEdit::focusOut, this, [&]() { hotkey->setRegistered(true); });

    connect(hotkey, &QHotkey::activated, this, &MainWindow::shortcutTriggered);

    // connect(floatBtn, &QtMaterialFloatingActionButton::clicked, this, [&]() {
    //     if (settingsWindow == nullptr) {
    //         settingsWindow = new SettingsWindow(this);

    //         connect(settingsWindow, &SettingsWindow::closed, this, [&]() { settingsWindow = nullptr; });
    //         settingsWindow->show();
    //     }
    // });
}

void MainWindow::loadSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat);

    // if (settings.value("autoToggle", false).toBool()) {
    //     autoToggle->setChecked(true);
    // }

    registerShortcut(settings.value("shortcut", "Ctrl+Shift+C").toString());
}

void MainWindow::saveSettings()
{
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);

    //settings.setValue("autoToggle", autoToggle->isChecked());
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
