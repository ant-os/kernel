#pragma once

typedef int(CMDHandler)(int, char**);

struct CommandHandler
{
    char* Name;
    char* Desc;
    char* Syntax;

    CMDHandler Handler;
};

void InitializeCommandList()
void RegisterCommandHandler(char* name, char* desc,char* syntax, CMDHandler handler);
