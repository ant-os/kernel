#include "apic.h"

ACPI::MADTTable* g_MADT = nullptr;

void APIC::Init(ACPI::MADTTable* madt)
{
    g_MADT = madt;
}

ACPI::MADTTable* APIC::GetMADTTable()
{
    return g_MADT;
}
