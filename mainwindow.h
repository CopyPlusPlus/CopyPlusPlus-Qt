#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qhotkey.h"
#include "qsettings.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void ininGui();
    void loadSettings();
    void saveSettings();
    void registerHotkey(bool status);
    void processClipboard();
    void closeEvent(QCloseEvent *event);

private:
    bool flag = false;

    Ui::MainWindow *ui;
    QHotkey *hotkey;

    QString settingsIniFile;
    QSettings settings;
    void afterChanged();
    void pressCtrlC();
};
#endif // MAINWINDOW_H
