#include "Session.h"


SESSION_ID g_sessionID = 0;
SESSION_ID g_parentID = 0;

void ExitSession(SESSION_ID SessionID)
{
    g_sessionID = (SessionID - 1);
}

void NewSession(){
    g_parentID = g_sessionID;
    ++g_sessionID;
}

SESSION_ID ActiveSessionID()
{
    return SESSION_ID(g_sessionID);
}

