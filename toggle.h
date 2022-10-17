#ifndef TOGGLE_H
#define TOGGLE_H

#include <QWidget>

class QtMaterialToggle;

namespace Ui
{
    class Toggle;
}

class Toggle : public QWidget
{
    Q_OBJECT

public:
    explicit Toggle(QWidget *parent = nullptr);
    explicit Toggle(QString _name, QWidget *parent = nullptr);
    ~Toggle();
    bool isChecked();
    void toggle();
    void setChecked(bool status);
    QString getName();
    void setName(QString name);

private:
    void initToggle();

public:
    QtMaterialToggle *m_toggle;

private:
    Ui::Toggle *ui;
};

#endif // TOGGLE_H
