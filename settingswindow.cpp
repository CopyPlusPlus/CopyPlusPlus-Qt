#include "settingswindow.h"
#include "mainwindow.h"
#include "qdebug.h"
#include "qtranslator.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent),
                                                  ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    allLanguages = QStringList{tr("中文"), tr("英文")};
    languageName = {{tr("英文"), "en_US"}};

    updateText();

    // 函数指针
    void (QComboBox::*currentIndexChanged)(const QString &) = &QComboBox::currentIndexChanged;
    connect(ui->languageList, currentIndexChanged, this, &SettingsWindow::SetLangusge);
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
        list->addItems(allLanguages);
    } else {
        for (int i = 0; i < allLanguages.count(); ++i) {
            list->setItemText(i, tr(allLanguages[i].toUtf8()));
        }
    }
}

// multi languages
void SettingsWindow::SetLangusge(const QString newLang)
{
    qDebug() << newLang;
    QTranslator *translator = MainWindow::getTranslator();

    if (newLang == tr("中文")) {
        qApp->removeTranslator(translator);
    } else {
        const QString baseName = "CopyPlusPlus-Qt_" + languageName[newLang];

        if (translator->load(":/i18n/" + baseName)) {
            qApp->installTranslator(translator);
        }
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
