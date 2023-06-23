#pragma once
#include <ant/string.h>

#include "Console.h"
#include "cstr.h"
#include "kernel.h"

/// @brief Command Handler Callback Function!
/// @param argc
/// @param **argv
/// @param *running
/// @return EXIT_SUCESS or EXIT_FAILURE
typedef int(CMDHandler)(int, char**, bool*, EnvironmentVariable**);


int Handle_VerCMD(int argc, char** argv, bool* running , EnvironmentVariable**);
int Handle_ClsCMD(int argc, char** argv, bool* running , EnvironmentVariable**);
int Handle_DateCMD(int argc, char** argv, bool* running, EnvironmentVariable**);
int Handle_TimeCMD(int argc, char** argv, bool* running, EnvironmentVariable**);
int Handle_EchoCMD(int argc, char** argv, bool* running, EnvironmentVariable**);