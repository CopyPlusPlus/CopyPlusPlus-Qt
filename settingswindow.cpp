#include "settingswindow.h"
#include "qdebug.h"
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

void SettingsWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }

    QMainWindow::changeEvent(event);
}

void SettingsWindow::SetLangusge(QString newLang)
{
    // multi languages
    const QString baseName = "CopyPlusPlus-Qt_en_US";

    if (translator.load(":/i18n/" + baseName)) {
        qApp->installTranslator(&translator);
    }

    const QStringList uiLanguages = QLocale::system().uiLanguages();

    //    for (const QString &locale : uiLanguages) {
    //        const QString baseName = "CopyPlusPlus-Qt_" + QLocale(locale).name();
    //        if (translator.load(":/i18n/" + baseName)) {
    //            // a.installTranslator(&translator);
    //            break;
    //        }
    //    }
}
