#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "mykeysequenceedit.h"
#include <QMainWindow>

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;

public:
    MyKeySequenceEdit *ke;
};

#endif // SETTINGSWINDOW_H
