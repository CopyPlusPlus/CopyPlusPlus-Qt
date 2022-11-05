#ifndef MYKEYSEQUENCEEDIT_H
#define MYKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>
#include <QLineEdit>
#include <QWidget>

class myKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    myKeySequenceEdit(QWidget *parent = nullptr);

    void focusInEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *) override;
    void clear();

private:
    // void resetState();

public:
    QLineEdit *lineEdit;

signals:
    void focusIn();
};

#endif // MYKEYSEQUENCEEDIT_H
