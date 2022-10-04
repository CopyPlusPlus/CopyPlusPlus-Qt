#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qhotkey.h"
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

private:
    void ininGui();
    void loadSettings();
    void saveSettings();
    void process();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QHotkey *hotkey;

    QString settingsIniFile;
    QSettings settings;
    void afterChanged();
};
#endif // MAINWINDOW_H
