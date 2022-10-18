#include "mykeysequenceedit.h"

myKeySequenceEdit::myKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent) {}

void myKeySequenceEdit::focusInEvent(QFocusEvent *event)
{
    emit focusIn();
    QKeySequenceEdit::focusInEvent(event);
}
