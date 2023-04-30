#ifndef TOGGLE_H
#define TOGGLE_H

#include "qtmaterialtoggle.h"

#include <QLabel>

class MyToggle : public QWidget
{
    Q_OBJECT

public:
    explicit MyToggle(QString _name, QWidget *parent = nullptr);
    ~MyToggle();

    bool isChecked();
    void setChecked(bool status);

    QString getName();
    void setName(QString name);

private:
    QLabel *label;
    QtMaterialToggle *toggle;
};

#endif // TOGGLE_H
