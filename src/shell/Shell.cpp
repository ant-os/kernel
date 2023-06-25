#include "Shell.h"
#include "Session.h"
#include "Util.h"
#include "Parser.h"

bool g_IsRunning = false;

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

    Console::WriteFormat("%s {%u / %u bytes}\n", CommandLine, CommandLineBytes, INPUT_BUFFER_SIZE + 1);

}

bool IsShellRunning()
{
    return g_IsRunning;
}
