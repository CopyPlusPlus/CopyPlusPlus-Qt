#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "processtext.h"
#include "qsettings.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void disConnect();

private:
    void ininGui();
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QString settingsIniFile;
    QSettings settings;
    ProcessText pro;
};
#endif // MAINWINDOW_H
