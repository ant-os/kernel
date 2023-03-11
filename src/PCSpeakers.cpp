#include "PCSpeakers.h"

static bool isEnabled = false;

void Sound::Speakers::Enable()
{
    ShowInfo(COM1, "\r\nPC Speakers are now Enabled...\n\r");

    isEnabled = true;
}

void Sound::Speakers::Disable()
{
    ShowInfo(COM1, "\r\nPC Speakers are now Disabled...\n\r");

    isEnabled = false;
}

void Sound::Speakers::Play(uint32_t nFrequence)
{
    uint32_t Div;
    uint8_t tmp;

    //Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(Div));
    outb(0x42, (uint8_t)(Div >> 8));

    //And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

void Sound::Speakers::NoSound()
{
    uint8_t tmp = inb(0x61) & 0xFC;

    outb(0x61, tmp);
}

static uint32_t bFrequency = 100;
static uint64_t bTime = 10;
void Sound::Speakers::Beep()
{
    if(isEnabled){
    Play(bFrequency);
    PIT::Sleep(bTime);
    NoSound();
    }
}

void Sound::Speakers::SetFrequency(uint32_t frequency)
{
    bFrequency = frequency;
}

uint32_t Sound::Speakers::GetFrequency()
{
    return bFrequency;
}

void Sound::Speakers::SetBeepTime(uint64_t time)
{
    bTime = time;
}

uint32_t Sound::Speakers::GetBeepTime()
{
    return bTime;
}
