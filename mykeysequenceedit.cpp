#include "mykeysequenceedit.h"
#include "qdebug.h"
#include "qevent.h"
#include <QLineEdit>

MyKeySequenceEdit::MyKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent)
{
    setStyleSheet("background-color: transparent; border: 1px solid white;");
    setFocusPolicy(Qt::ClickFocus);

    lineEdit = findChild<QLineEdit *>("qt_keysequenceedit_lineedit", Qt::FindDirectChildrenOnly);
    lineEdit->setCursor(Qt::ArrowCursor);
    lineEdit->setAlignment(Qt::AlignHCenter);
    lineEdit->setPlaceholderText(QKeySequenceEdit::tr("快捷键"));

    // setKeySequence 时会 resetState，从而 reset PlaceholderText
    // 需要重新 setPlaceholderText 避免出现默认的"Press shortcut"

    QString seq = settings.value("shortcut", "Ctrl+Shift+C").toString();
    if (!seq.isEmpty()) {
        setKeySequence(QKeySequence(seq));
    }
}

void MyKeySequenceEdit::focusInEvent(QFocusEvent *event)
{
    setStyleSheet("border: 1px solid blue;");

    qDebug() << "Focus" << event->reason();

    // 非主动 setFocus() 时，清空内容
    if (event->reason() != Qt::OtherFocusReason) {
        clear();
    }
}

void MyKeySequenceEdit::focusOutEvent(QFocusEvent *event)
{
    // 非主动 clearFocus() 时
    if (event->reason() != Qt::OtherFocusReason) {
        setStyleSheet("border: 1px solid white;");
    }

    qDebug() << "FocusOut" << event->reason();
}

void MyKeySequenceEdit::keyPressEvent(QKeyEvent *event)
{
    QKeySequenceEdit::keyPressEvent(event);
    if (this->keySequence().count() > 0) {
        QKeySequenceEdit::setKeySequence(this->keySequence());

        clearFocus();
        lineEdit->setPlaceholderText("快捷键");

        qDebug() << "66" << this->keySequence().toString();
        settings.setValue("shortcut", this->keySequence().toString());

        // emit myEditFinished(this->keySequence());

        setFocus();
    }
}

void MyKeySequenceEdit::clear()
{
    QKeySequenceEdit::clear();
    lineEdit->setPlaceholderText(QKeySequenceEdit::tr("快捷键"));
}
