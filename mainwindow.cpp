#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setFixedSize(420, 360);
    ui->toggle1->setName("自动合并");
    ui->toggle2->setName("快捷键合并");
}

MainWindow::~MainWindow() {
    delete ui;
}
