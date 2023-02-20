#include "settingswindow.h"
#include "qdebug.h"
#include "qtranslator.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent),
                                                  ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    languages = QStringList{tr("中文"), tr("英文")};
    updateText();

    connect(ui->languageList, &QComboBox::currentTextChanged, this, &SettingsWindow::SetLangusge);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        // 感觉没啥用
        ui->retranslateUi(this);

        // 更新文本，用于翻译
        updateText();
    }

    QMainWindow::changeEvent(event);
}

void SettingsWindow::updateText()
{
    setWindowTitle(tr("设置"));

    ui->languageLable->setText(tr("语言"));

    QComboBox *list = ui->languageList;

    if (list->count() == 0) {
        list->addItems(languages);
    } else {
        for (int i = 0; i < languages.count(); ++i) {
            list->setItemText(i, tr(languages[i].toUtf8()));
        }
    }
}

void SettingsWindow::SetLangusge(QString newLang)
{
    qDebug() << newLang;

    // multi languages
    const QString baseName = "CopyPlusPlus-Qt_en_US";

    if (translator.load(":/i18n/" + baseName)) {
        qApp->installTranslator(&translator);
    }

    //    const QStringList uiLanguages = QLocale::system().uiLanguages();

    //    for (const QString &locale : uiLanguages) {
    //        const QString baseName = "CopyPlusPlus-Qt_" + QLocale(locale).name();
    //        if (translator.load(":/i18n/" + baseName)) {
    //            // a.installTranslator(&translator);
    //            break;
    //        }
    //    }
}
