#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    //  High DPI
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    QApplication a(argc, argv);

    // multi languages
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CopyPlusPlus-Qt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QFont font("Source Han Sans");
    font.setStyleHint(QFont::SansSerif);
    QApplication::setFont(font);

    MainWindow w;
    w.show();
    return a.exec();
}
