#include "mykeysequenceedit.h"
#include "qdebug.h"
#include "qevent.h"
#include <QLineEdit>

MyKeySequenceEdit::MyKeySequenceEdit(QWidget *parent) : QKeySequenceEdit(parent)
{
    setStyleSheet("background-color: transparent; border: 1px solid white;");
    setFocusPolicy(Qt::ClickFocus);

    // 获取组件中的 QLineEdit
    lineEdit = findChild<QLineEdit *>("qt_keysequenceedit_lineedit", Qt::FindDirectChildrenOnly);

    lineEdit->setCursor(Qt::ArrowCursor);              // 光标设为箭头
    lineEdit->setAlignment(Qt::AlignHCenter);          // 居中
    lineEdit->setContextMenuPolicy(Qt::NoContextMenu); // 禁用右键菜单

    // setKeySequence 时会 resetState, 从而 reset PlaceholderText
    // 需要重新 setPlaceholderText 避免出现默认的"Press shortcut"
    lineEdit->setPlaceholderText(QKeySequenceEdit::tr("Hotkey"));

    QString seq = settings.value("shortcut", "Ctrl+Shift+C").toString();
    qDebug() << "shortcut: " << seq;
    if (!seq.isEmpty()) {
        setKeySequence(QKeySequence(seq));
    }
}

void MyKeySequenceEdit::focusInEvent(QFocusEvent *event)
{
    setStyleSheet("border: 1px solid blue;");
    emit focusIn();

    event->accept();
}

void MyKeySequenceEdit::focusOutEvent(QFocusEvent *event)
{
    // 非 clearFocus() 时
    if (event->reason() != Qt::OtherFocusReason) {
        setStyleSheet("border: 1px solid white;");

        if (this->keySequence().isEmpty()) {
            setKeySequence(QKeySequence(settings.value("shortcut", "Ctrl+Shift+C").toString()));
            lineEdit->setPlaceholderText(tr("Hotkey"));
        }

        emit focusOut();
    }
}

void MyKeySequenceEdit::keyPressEvent(QKeyEvent *event)
{
    QKeySequenceEdit::keyPressEvent(event);

    if (event->key() == Qt::Key_Backspace) {
        clear();
        emit myEditFinished(this->keySequence());
    }

    if (this->keySequence().count() > 0) {
        setKeySequence(this->keySequence());

        clearFocus();
        lineEdit->setPlaceholderText(tr("Hotkey"));

        emit myEditFinished(this->keySequence());
        qDebug() << "myEditFinished";

        settings.setValue("shortcut", this->keySequence().toString());

        setFocus();
    }
}

void MyKeySequenceEdit::keyReleaseEvent(QKeyEvent *event)
{
    QKeySequenceEdit::keyReleaseEvent(event);

    //    if (this->keySequence().isEmpty()) {
    //        setKeySequence(QKeySequence(settings.value("shortcut", "Ctrl+Shift+C").toString()));
    //        lineEdit->setPlaceholderText(tr("快捷键"));
    //    }
}

void MyKeySequenceEdit::clear()
{
    QKeySequenceEdit::clear();
    lineEdit->setPlaceholderText(QKeySequenceEdit::tr("Hotkey"));
}
