#include "CommandHandlers.h"
#include "RealTimeClock.h"

int Handle_VerCMD(int argc, char** argv, bool* running, EnvironmentVariable**) {


    printf("AntOS %s [ %s ]\n", __GIT_VERSION__, __COMPILE_DATETIME__);

    return EXIT_SUCESS;
};

int Handle_ClsCMD(int argc, char** argv, bool* running, EnvironmentVariable**) {
    printf("\xEC\xEE");

    return EXIT_SUCESS;
}

int Handle_DateCMD(int argc, char** argv, bool* running, EnvironmentVariable**) {
    RTC::ClockStatus clock = RTC::Read();
    printf("Current Date: %u/%u/%u", (uint64_t)clock.month, (uint64_t)clock.day, clock.year);

    return EXIT_SUCESS;
}

int Handle_TimeCMD(int argc, char** argv, bool* running, EnvironmentVariable**) {
    RTC::ClockStatus clock = RTC::Read();
    printf("Current Date: %u/%u/%u", (uint64_t)clock.month, (uint64_t)clock.day, clock.year);

    return EXIT_SUCESS;
}

int Handle_EchoCMD(int argc, char** argv, bool* running, EnvironmentVariable** envVars) {


    if (argc >= 1) {
        char* input = argv[1];

        // printf("%x\n", (uint64_t)envVars[0]->value);

        printf("%s", argv[1]);

        return EXIT_SUCESS;
    }

    return EXIT_FAILURE;
}