#include "mainwindow.h"
#include "processtext.h"
#include "ui_mainwindow.h"
#include <QClipboard>
#include <QCloseEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
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

        connect(QGuiApplication::clipboard(), &QClipboard::changed, &pro, &ProcessText::process);
    }
}

void MainWindow::saveSettings() {
    // QSettings settings(settingsIniFile, QSettings::IniFormat, this);
    settings.setValue("toggle1", ui->toggle1->isChecked());
    settings.setValue("toggle2", ui->toggle2->isChecked());
}

void MainWindow::disConnect() {
    // disconnect(QGuiApplication::clipboard(), &QClipboard::changed, &pro, &ProcessText::process);
}
