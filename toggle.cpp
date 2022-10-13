#include "toggle.h"
#include "ui_toggle.h"
#include <QHBoxLayout>
#include <qtmaterialtoggle.h>

Toggle::Toggle(QWidget *parent) : QWidget(parent), ui(new Ui::Toggle) {
    ui->setupUi(this);
    initToggle();
}

Toggle::Toggle(QString name, QWidget *parent) : QWidget(parent), ui(new Ui::Toggle) {
    ui->setupUi(this);
    ui->label->setText(name);
    initToggle();
}

Toggle::~Toggle() {
    delete ui;
}

void Toggle::initToggle() {
    m_toggle = new QtMaterialToggle;

    auto h = new QHBoxLayout();
    ui->widget->setLayout(h);
    h->setContentsMargins(0, 0, 0, 0);
    h->addWidget(m_toggle, Qt::AlignRight);
}

bool Toggle::isChecked() {
    return m_toggle->isChecked();
}

void Toggle::toggle() {
    m_toggle->setChecked(!m_toggle->isChecked());
}

void Toggle::setChecked(bool status) {
    m_toggle->setChecked(status);
}

QString Toggle::getName() {
    return ui->label->text();
}

void Toggle::setName(QString name) {
    ui->label->setText(name);
}

void Toggle::connectToggled(bool (*action)(bool)) {
    connect(this->m_toggle, &QtMaterialToggle::toggled, qApp, action);
}
