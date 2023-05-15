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
