#pragma once
#include "acpi.h"

namespace APIC {
    void Init(ACPI::MADTTable* madt);
    ACPI::MADTTable* GetMADTTable();
};