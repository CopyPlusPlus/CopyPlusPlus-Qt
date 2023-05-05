#include "textprocessor.h"
#include "qclipboard.h"
#include "qmimedata.h"

#include <QDebug>
#include <QGuiApplication>
#include <QSettings>
#include <QThread>

#ifdef Q_OS_WIN
#include <windows.h>
#include <winuser.h>
#endif

#ifdef Q_OS_MAC
#include <Carbon/Carbon.h>
#endif

void TextProcessor::processClipboard()
{
    QThread::msleep(50);

    // 防止截图等仅复制图片时处理导致无法复制
    if (!QGuiApplication::clipboard()->mimeData()->hasText()) {
        return;
    }

    QString s = QGuiApplication::clipboard()->text();

    qDebug() << "Before:" << s;

    // 去掉 CAJ viewer 造成的莫名的空格符号
    s.replace("", "");

    s.replace("\r", "");
    s.replace("\n", "");

    setClipboardText(s);

    qDebug() << "After:" << QGuiApplication::clipboard()->text();
}

void TextProcessor::setClipboardText(QString _text)
{
#ifdef Q_OS_MAC
    QGuiApplication::clipboard()->setText(_text);
#elif defined(Q_OS_WIN)
    QByteArray ba = _text.toLocal8Bit();
    const char *text = ba.data();
    const size_t len = strlen(text) + 1;

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);

    while (!OpenClipboard(0)) {
        qDebug() << "Cannot open clipboard";
        Sleep(50);
    }

    while (!EmptyClipboard()) {
        qDebug() << "Cannot empty clipboard";
        Sleep(50);
    }

    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
#endif
}

void TextProcessor::pressCtrlC()
{
    QSettings settings;

#ifdef Q_OS_WIN

    // 获取当前绑定快捷键 Modifier + Key
    QStringList hotKeys = settings.value("shortcut", "Ctrl+Shift+C").toString().split("+");

    const int n = hotKeys.size();

    // 需要模拟的按键事件数组, 共需要模拟：快捷键释放:n个；Ctrl+C按下、释放:4个
    // 恢复时只需要按下Modifier, 所以总个数为 2*n+4-1 = 2*n+3
    const int keysNum = 2 * n + 3;

    INPUT *inputs = new INPUT[keysNum];
    ZeroMemory(inputs, keysNum * sizeof(INPUT));

    // 释放 Modifier
    for (int i = 0; i < n - 1; ++i) {
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;

        switch (hotKeys[i].toStdString()[0]) {
        case 'C':
            inputs[i].ki.wVk = VK_LCONTROL;
            break;
        case 'S':
            inputs[i].ki.wVk = VK_LSHIFT;
            break;
        case 'M':
            inputs[i].ki.wVk = VK_LWIN;
            break;
        case 'A':
            inputs[i].ki.wVk = VK_LMENU;
            break;
        }
    }
    // 释放 key
    inputs[n - 1].type = INPUT_KEYBOARD;
    inputs[n - 1].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[n - 1].ki.wVk = VkKeyScanA(hotKeys[n - 1].toStdString()[0]);

    // 按下 CTRL
    inputs[n].type = INPUT_KEYBOARD;
    inputs[n].ki.wVk = VK_LCONTROL;
    inputs[n].ki.dwFlags = 0;
    // 按下 C
    inputs[n + 1].type = INPUT_KEYBOARD;
    inputs[n + 1].ki.wVk = 0x43; // C
    inputs[n + 1].ki.dwFlags = 0;
    // 释放 CTRL
    inputs[n + 2].type = INPUT_KEYBOARD;
    inputs[n + 2].ki.wVk = VK_LCONTROL;
    inputs[n + 2].ki.dwFlags = KEYEVENTF_KEYUP;
    // 释放 C
    inputs[n + 3].type = INPUT_KEYBOARD;
    inputs[n + 3].ki.wVk = 0x43; // C
    inputs[n + 3].ki.dwFlags = KEYEVENTF_KEYUP;

    // 恢复按下 Modifier
    for (int i = n + 4; i < keysNum; ++i) {
        inputs[i].type = INPUT_KEYBOARD;
        inputs[i].ki.dwFlags = 0;

        switch (hotKeys[i - n - 4].toStdString()[0]) {
        case 'C':
            inputs[i].ki.wVk = VK_LCONTROL;
            break;
        case 'S':
            inputs[i].ki.wVk = VK_LSHIFT;
            break;
        case 'M':
            inputs[i].ki.wVk = VK_LWIN;
            break;
        case 'A':
            inputs[i].ki.wVk = VK_LMENU;
            break;
        }
    }

    // 模拟按键
    UINT uSent = SendInput(keysNum, inputs, sizeof(INPUT));
    if (uSent != keysNum) {
        qDebug() << "SendInput failed:" << HRESULT_FROM_WIN32(GetLastError());
    } else {
        qDebug() << "SendInput succeed";
    }

    delete[] inputs;

#elif defined(Q_OS_MAC)

    CGKeyCode inputKeyCode = kVK_ANSI_C;
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateCombinedSessionState);
    CGEventRef saveCommandDown = CGEventCreateKeyboardEvent(source, inputKeyCode, true);
    CGEventSetFlags(saveCommandDown, kCGEventFlagMaskCommand);
    CGEventRef saveCommandUp = CGEventCreateKeyboardEvent(source, inputKeyCode, false);

    CGEventPost(kCGAnnotatedSessionEventTap, saveCommandDown);
    CGEventPost(kCGAnnotatedSessionEventTap, saveCommandUp);

    CFRelease(saveCommandUp);
    CFRelease(saveCommandDown);
    CFRelease(source);

#endif
}
