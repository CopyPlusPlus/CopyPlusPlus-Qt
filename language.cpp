#include "language.h"
#include "mainwindow.h"
#include "qapplication.h"
#include <QHash>
#include <QSettings>
#include <QString>
#include <QTranslator>

QStringList Language::allLanguages = {"中文", "English"};

QHash<QString, QString> Language::languageName = {
    {"中文", "zh_CN"},
    {"English", "en_US"}};

void Language::initLanguage()
{
    QSettings settings;

    // if language setting saves as number (initial version)
    //    if (std::isdigit(settings.value("language").toString().toStdString()[0])) {
    //        settings.remove("language");
    //    }

    // if no language setting, read system language
    if (!settings.contains("language")) {
        QString sysLang = QLocale::system().name();

        qDebug() << "System language:" << sysLang;

        settings.setValue("language", sysLang);
    }

    qDebug() << "Language setting:" << settings.value("language", "0");

    updateLanguage(settings.value("language", "0").toInt());
}

void Language::updateLanguage(const int &newLangIndex)
{
    QSettings settings;
    settings.setValue("language", newLangIndex);

    QTranslator *translator = MainWindow::getInstance()->translator;
    qApp->removeTranslator(translator);

    if (newLangIndex != 0) {
        const QString baseName = "CopyPlusPlus-Qt_" + languageName[allLanguages[newLangIndex]];

        if (translator->load(":/i18n/" + baseName)) {
            qApp->installTranslator(translator);
        }
    }
}
