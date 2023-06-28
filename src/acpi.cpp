#include "acpi.h"
#include "Serial.h"

namespace ACPI {

    void* FindTable(SDTHeader* sdtHeader, char* signature) {

        Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);

        int entries = (sdtHeader->Length - sizeof(ACPI::SDTHeader)) / 8;

        Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);

        for (int t = 0; t < entries; t++) {
            Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);
            ACPI::SDTHeader* newSDTHeader = (ACPI::SDTHeader*)*(uint64_t*)((uint64_t)sdtHeader + sizeof(ACPI::SDTHeader) + (t * 8));
            Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);
            for (int i = 0; i < 4; i++) {
                Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);
                if (newSDTHeader->Signature[i] != signature[i])
                {
                    Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);
                    break;
                }
                Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);
                if (t == 3) return newSDTHeader;
            }
        }
        Serial::WriteFormat(COM1, "\n::%d::\n", __LINE__);
        return 0;
    }
    
}