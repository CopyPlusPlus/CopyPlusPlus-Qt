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
