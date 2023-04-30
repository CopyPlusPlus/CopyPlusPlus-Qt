#include "mytoggle.h"
#include "qtmaterialtoggle.h"

#include <QHBoxLayout>
#include <QtWidgets>

MyToggle::MyToggle(QString name, QWidget *parent) : QWidget(parent)
{
    setFixedHeight(48);

    label = new QLabel(name);
    toggle = new QtMaterialToggle;

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(label, 0, Qt::AlignLeft);
    layout->addWidget(toggle, 0, Qt::AlignRight);

    setFixedSize(layout->sizeHint());
    setMinimumSize(layout->sizeHint());
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
