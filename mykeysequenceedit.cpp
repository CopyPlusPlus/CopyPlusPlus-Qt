#include "mykeysequenceedit.h"
#include <QLineEdit>

myKeySequenceEdit::myKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent)
{
    setStyleSheet("background-color: transparent;"
                  "qproperty-frame: true;");
    setFocusPolicy(Qt::NoFocus);

    lineEdit = findChild<QLineEdit *>("qt_keysequenceedit_lineedit", Qt::FindDirectChildrenOnly);
    lineEdit->setCursor(Qt::ArrowCursor);
    lineEdit->setAlignment(Qt::AlignHCenter);

    lineEdit->setPlaceholderText(QKeySequenceEdit::tr("快捷键"));
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

void myKeySequenceEdit::clear()
{
    QKeySequenceEdit::clear();
    lineEdit->setPlaceholderText(QKeySequenceEdit::tr("快捷键"));
}
