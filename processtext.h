#ifndef PROCESSTEXT_H
#define PROCESSTEXT_H

#include <QObject>
#include <QString>

class ProcessText : public QObject {
    Q_OBJECT
public:
    explicit ProcessText(QObject *parent = nullptr);

public slots:
    void process();

signals:
};

#endif // PROCESSTEXT_H
