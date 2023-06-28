#include "keyboard.h"
#include <ant/memory.h>

bool isLeftShiftPressed;
bool isRightShiftPressed;

int64_t InputBufferPosition = INPUT_INACTIVE_POS;
char InputBuffer[INPUT_BUFFER_SIZE] = {0};

void HandleKeyboard(uint8_t scancode)
{

    bool allowInput = !(InputBufferPosition < 0);

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
        InputBufferPosition = INPUT_INACTIVE_POS;
        return;
    case Spacebar:
        if (allowInput){
        GlobalRenderer->PutChar(' ');
        Serial::Write(COM1, ' ');
        InputBuffer[InputBufferPosition] = ' ';
        ++InputBufferPosition;
        }
        return;
    case BackSpace:
        GlobalRenderer->ClearChar();
        InputBuffer[InputBufferPosition] = '\0';
        if (InputBufferPosition == INPUT_BLOCKED_POS){
            InputBufferPosition = (INPUT_BUFFER_SIZE - 1);
            break;
        }
        --InputBufferPosition;
        InputBuffer[InputBufferPosition] = '\0';
        return;
    }

    if (!allowInput) return;

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0)
    {
        GlobalRenderer->FillRect(GlobalRenderer->CursorPosition, 8, 16, 0x000000);
        GlobalRenderer->PutChar(ascii);
        GlobalRenderer->PutChar('\x5F', GlobalRenderer->CursorPosition.X, GlobalRenderer->CursorPosition.Y);

         /* Serial::WriteFormat(COM1, " %d ==> %d ==> %c ", scancode, ascii, ascii); */

        if (InputBufferPosition < INPUT_BUFFER_SIZE){
            InputBuffer[InputBufferPosition] = ascii;
            ++InputBufferPosition;
        } else{
            ShowError(COM1, "\nFailed to put inputed key(ascii) into the input buffer!\nNot enough space left!\nBlocking Input!!");
            InputBufferPosition = INPUT_BLOCKED_POS;
        }
    }
}

void ClearInputBuffer()
{
    int32_t oldPos = InputBufferPosition;

    InputBufferPosition = INPUT_BLOCKED_POS;

    ZeroMemory(InputBuffer, (INPUT_BUFFER_SIZE * sizeof(char)));

    InputBufferPosition = oldPos;
}
