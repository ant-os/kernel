#pragma once
#include <stdint.h>


// <==== Session Managment ====> //

typedef uint64_t SESSION_ID;

void NewSession(void);
void ExitSession(SESSION_ID SessionID);
SESSION_ID ActiveSessionID();