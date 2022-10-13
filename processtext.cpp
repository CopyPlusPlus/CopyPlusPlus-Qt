#include "processtext.h"
#include "qclipboard.h"
#include "qdebug.h"
#include <QGuiApplication>
#include <QString>

ProcessText::ProcessText(QObject *parent) : QObject{parent} {}

void ProcessText::process()
{
    qDebug() << "Clipboard changed";

    QString s = QGuiApplication::clipboard()->text();
    // s.replace("\r\n", "");
    qDebug() << "Before :" << s;

    QGuiApplication::clipboard()->setText(s);
}
