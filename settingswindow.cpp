#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent),
                                                  ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));

    setFocusPolicy(Qt::ClickFocus);
    ke = ui->keySequenceEdit;
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}
