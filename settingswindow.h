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
    void SetLangusge(const QString);
    void updateText();

    void changeEvent(QEvent *event);

private:
    Ui::SettingsWindow *ui;

    QStringList allLanguages;
    QHash<QString, QString> languageName;
};

#endif // SETTINGSWINDOW_H
