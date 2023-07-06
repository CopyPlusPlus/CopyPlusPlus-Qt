#ifndef TOGGLE_H
#define TOGGLE_H

#include <QWidget>

namespace Ui {
    class Toggle;
}

class Toggle : public QWidget
{
    Q_OBJECT

public:
    explicit Toggle(QWidget *parent = nullptr);
    ~Toggle();

    void setChecked(bool checked);
    bool isChecked();

    void setText(const QString &text);

signals:
    void toggled(bool checked);

private:
    Ui::Toggle *ui;
};

#endif // TOGGLE_H
