#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

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
    void closeEvent(QCloseEvent *event);

signals:
    void closed();

private:
    Ui::SettingsWindow *ui;

    QStringList allLanguages;
    QHash<QString, QString> languageName;
};

#endif // SETTINGSWINDOW_H
