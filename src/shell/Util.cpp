#include "Util.h"
#include "../Console.h"

void Exec(std::string commandLine)
{
    if (commandLine == "ver") Console::WriteFormat("AntOS %s [%s]\n\r", __GIT_VERSION__, __COMPILE_DATETIME__);
}
