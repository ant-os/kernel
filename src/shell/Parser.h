#pragma once

struct CommandInfo
{
    int size;
    char* cmd;
    int argc;
    char** argv;
};


class Parser{
    public:

        char* GetCommandString(char* line);
        char** GetArgValues(char* line);
        int GetArgCount(char* line);
        CommandInfo* GetCommandInfo(char* line);
};