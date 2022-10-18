#ifndef MYKEYSEQUENCEEDIT_H
#define MYKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>
#include <QWidget>

class myKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    myKeySequenceEdit(QWidget *parent = nullptr);

    void focusInEvent(QFocusEvent *event);

signals:
    void focusIn();
};

#endif // MYKEYSEQUENCEEDIT_H
