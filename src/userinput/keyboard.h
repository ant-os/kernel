#pragma once
#include <stdint.h>
#include "kbScancodeTranslation.h"
#include "../BasicRenderer.h"
#include "../Serial.h"

#define INPUT_BUFFER_SIZE 255

#define INPUT_INACTIVE_POS -1
#define INPUT_BLOCKED_POS -2
#define INPUT_ACTIVE_POS 0

#define KeyboardInProcess() InputBufferPosition != -1

extern char InputBuffer[INPUT_BUFFER_SIZE];
extern int64_t InputBufferPosition;

void HandleKeyboard(uint8_t scancode);
void ClearInputBuffer();