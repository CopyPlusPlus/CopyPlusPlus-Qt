#include "toggle.h"
#include "ui_toggle.h"

Toggle::Toggle(QWidget *parent) :
                                  QWidget(parent),
                                  ui(new Ui::Toggle)
{
    ui->setupUi(this);
}

Toggle::~Toggle()
{
    delete ui;
}

void Toggle::setChecked(bool checked)
{
    ui->toggle->setChecked(checked);

    emit toggled(checked);
}

bool Toggle::isChecked()
{
    return ui->toggle->isChecked();
}

void Toggle::setText(const QString &text)
{
    ui->label->setText(text);
}