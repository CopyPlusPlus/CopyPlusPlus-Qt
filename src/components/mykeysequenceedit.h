#ifndef MYKEYSEQUENCEEDIT_H
#define MYKEYSEQUENCEEDIT_H

#include "qsettings.h"
#include <QKeySequenceEdit>
#include <QLineEdit>
#include <QWidget>

class MyKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    MyKeySequenceEdit(QWidget *parent = nullptr);

    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *) override;
    void clear();

private:
    // void resetState();
    void errorInput();

public:
    QLineEdit *lineEdit;

private:
    QSettings settings;

signals:
    void myEditFinished(const QKeySequence &keySequence);
    void focusIn();
    void focusOut();
};

#endif // MYKEYSEQUENCEEDIT_H
