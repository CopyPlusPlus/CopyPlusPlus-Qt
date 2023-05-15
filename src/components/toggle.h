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

private:
    Ui::Toggle *ui;
};

#endif // TOGGLE_H
