#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qhotkey.h"
#include "qsettings.h"
//#include "qtmaterialfab.h"
//#include "qtmaterialtoggle.h"
#include "qtranslator.h"
#include "settingswindow.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow *getInstance();

private:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

    void initUI();
    void updateText();
    void initConnections();
    void loadSettings();
    void saveSettings();

    void autoToggleChecked(bool status);
    // void toggleShortcutChecked(bool status);

    void registerShortcut(const QKeySequence &keySequence);
    void shortcutTriggered();

    void keySequenceEditFinished(const QKeySequence &keySequence);
    // void truncateShortcut();

    void errorInput();

    void afterChanged();

    void click();

public:
    QHotkey *hotkey;

    QTranslator *translator;

private:
    static MainWindow *instance;

    Ui::MainWindow *ui;

    //QtMaterialToggle *autoToggle;
    //QtMaterialFloatingActionButton *floatBtn;

    SettingsWindow *settingsWindow;

    // QString settingsIniFile;
    QSettings settings;

    // 用于粘贴板事件二次触发判断
    bool flag = false;
};
#endif // MAINWINDOW_H