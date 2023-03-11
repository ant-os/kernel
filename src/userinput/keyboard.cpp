#include "keyboard.h"

bool isLeftShiftPressed;
bool isRightShiftPressed;

void HandleKeyboard(uint8_t scancode)
{

    switch (scancode)
    {
    case LeftShift:
        isLeftShiftPressed = true;
        return;
    case LeftShift + 0x80:
        isLeftShiftPressed = false;
        return;
    case RightShift:
        isRightShiftPressed = true;
        return;
    case RightShift + 0x80:
        isRightShiftPressed = false;
        return;
    case Enter:
        GlobalRenderer->Next();
        Serial::Write(COM1, '\n\r');
        return;
    case Spacebar:
        GlobalRenderer->PutChar(' ');
        Serial::Write(COM1, ' ');
        return;
    case BackSpace:
        GlobalRenderer->ClearChar();
        Serial::Write(COM1, '\b');
        return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0)
    {
        GlobalRenderer->PutChar(ascii);
        Serial::Write(COM1, ascii);
    }
}