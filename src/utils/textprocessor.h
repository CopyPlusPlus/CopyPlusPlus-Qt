#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <QString>

class TextProcessor
{
public:
    static void processClipboard();
    static void setClipboardText(QString);
    static void pressCtrlC();
};

#endif // TEXTPROCESSOR_H
