#include "Shell.h"
#include "Session.h"
#include "Util.h"
#include "Parser.h"

bool g_IsRunning = false;
Parser g_Parser = Parser();

void InitializeShell()
{
    g_IsRunning = true;
   
    NewSession();
}

void ShellUpdate()
{
    if (!!!IsShellRunning()) return;

    char CommandLine[INPUT_BUFFER_SIZE + 1] = {0};
    size_t CommandLineBytes = 0;

    Console::WriteFormat(">>> ");

    ClearInputBuffer();

    CommandLineBytes = Console::ReadBuffer(INPUT_BUFFER_SIZE, (char**)&CommandLine);

    if (!!!CommandLineBytes) return;

    // Special Parsing... PIPE ( | )

    Exec(std::string::from(InputBuffer));

    CommandInfo* cmdInfo = g_Parser.GetCommandInfo(CommandLine);

    Console::WriteFormat("\bThe Command \"%s\" is invalid! {argc=%u;argv=%p}\n", cmdInfo->cmd, cmdInfo->argc, cmdInfo->argv);

}

bool IsShellRunning()
{
    return g_IsRunning;
}
