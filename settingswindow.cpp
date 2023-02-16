#include "settingswindow.h"
#include "qtranslator.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent),
                                                  ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));

    ui->languageLable->setText(tr("语言"));

    QStringList languages = {tr("中文"), tr("英文")};

    ui->languageList->addItems(languages);
    connect(ui->languageList, &QComboBox::currentTextChanged, this, &SettingsWindow::SetLangusge);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::SetLangusge(QString newLang)
{
    QLocale locale = QLocale(newLang);
    QLocale::setDefault(locale);
    QString languageName = QLocale::languageToString(locale.language());
    QTranslator translator;

    // remove the old translator
    qApp->removeTranslator(&translator);

    qApp->installTranslator(&translator);
}
