#include "mykeysequenceedit.h"

myKeySequenceEdit::myKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent)
{
    setStyleSheet("background-color: transparent;"
                  "qproperty-frame: false;");
    setFocusPolicy(Qt::NoFocus);
}

void myKeySequenceEdit::focusInEvent(QFocusEvent *event)
{
    emit focusIn();
    QKeySequenceEdit::focusInEvent(event);
}

void myKeySequenceEdit::keyPressEvent(QKeyEvent *event)
{
    QKeySequenceEdit::keyPressEvent(event);
    if (this->keySequence().count() > 0) {
        QKeySequenceEdit::setKeySequence(this->keySequence());
        emit editingFinished();
    }
}
