#include "language.h"
#include "mainwindow.h"
#include "qapplication.h"
#include <QHash>
#include <QSettings>
#include <QString>
#include <QTranslator>

QStringList Language::allLanguages = {"English", "中文"};

QHash<QString, QString> Language::languageName = {
    {"English", "en_US"},
    {"中文", "zh_CN"}};

void Language::initLanguage()
{
    QSettings settings;

    // if language setting saves as number (initial version)
    //    if (std::isdigit(settings.value("language").toString().toStdString()[0])) {
    //        settings.remove("language");
    //    }

    // If there is no language setting, read system language
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
        const QString baseName = "CopyPlusPlus_" + languageName[allLanguages[newLangIndex]];
        if (translator->load(baseName)) {
            qApp->installTranslator(translator);
        }
    }
}
