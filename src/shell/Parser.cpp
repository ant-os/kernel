#include "Parser.h"
#include <ant/dynamic.h>


char* Parser::GetCommandString(char* line)
{
    int space_pos = 0;

    for (size_t i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == ' ') break;;

        ++space_pos;
    }

    char* cmdBuffer = (char*)malloc(space_pos);
    
    if (cmdBuffer == nullptr) return nullptr;

    memcpy(line, cmdBuffer, space_pos);
    
    cmdBuffer[space_pos] = 0;

    return cmdBuffer;
}

char** Parser::GetArgValues(char* line)
{
    int argc = GetArgCount(line);
    int count = 0;

    /// TODO: Impl!

    return nullptr;
}


int Parser::GetArgCount(char* line)
{
    int count = 0;

    for (size_t i = 0; line[i] != '\0'; i++)
        if (line[i] == ' ' || (count > 0 && line[i+1]=='\0')) ++count;

    return (count == 0) ? 0 : (count - 1);
}

CommandInfo* Parser::GetCommandInfo(char* line)
{
    CommandInfo* info = (CommandInfo*)malloc(sizeof(CommandInfo));

    info->size = sizeof(CommandInfo);
    info->cmd = GetCommandString(line);
    info->argc = GetArgCount(line);
    info->argv = GetArgValues(line);

    return info;
}
