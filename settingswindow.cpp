#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent),
                                                  ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}
