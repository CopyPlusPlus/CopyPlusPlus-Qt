#include "mytoggle.h"
#include "qtmaterialtoggle.h"

#include <QFormLayout>
#include <QtWidgets>

MyToggle::MyToggle(QString name, QWidget *parent) : QWidget(parent)
{
    setFixedHeight(48);
    setFixedWidth(100);

    label = new QLabel(name);
    toggle = new QtMaterialToggle;

    setStyleSheet("border: 1px solid white");

    //auto layout = new QFormLayout(this);
    //layout->setContentsMargins(0, 0, 0, 0);

    //layout->addRow(label, toggle);

    //setFixedSize(layout->sizeHint());
    //setMinimumSize(layout->sizeHint());
}

MyToggle::~MyToggle()
{
}

bool MyToggle::isChecked()
{
    return toggle->isChecked();
}

void MyToggle::setChecked(bool status)
{
    toggle->setChecked(status);
}

void MyToggle::setName(QString name)
{
    label->setText(name);
}

QString MyToggle::getName()
{
    return label->text();
}
