#pragma once
#include <stdint.h>
#include <stddef.h>
#include "IO.h"
#include "scheduling/pit/pit.h"
#include "Serial.h"

namespace Sound
{
    namespace Speakers
    {
        void Enable();
        void Disable();
        void Play(uint32_t nFrequency);
        void NoSound();
        void Beep();
        void SetFrequency(uint32_t frequency);
        uint32_t GetFrequency();
        void SetBeepTime(uint64_t time);
        uint32_t GetBeepTime();
    } // namespace Speakers
    
} // namespace Sound
