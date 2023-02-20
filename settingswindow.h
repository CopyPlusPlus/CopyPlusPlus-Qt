#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "qtranslator.h"
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
    void SetLangusge(QString);
    void updateText();

    void changeEvent(QEvent *event);

private:
    Ui::SettingsWindow *ui;

    QStringList languages;
    QTranslator translator;
};

#endif // SETTINGSWINDOW_H
