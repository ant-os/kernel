#pragma once
#include <stdint.h>

namespace ACPI
{
    struct RSDP2
    {
        unsigned char Signature[8];
        uint8_t Checksum;
        uint8_t OEMId[6];
        uint8_t Revision;
        uint32_t RSDTAddress;
        uint32_t Length;
        uint64_t XSDTAddress;
        uint8_t ExtendedChecksum;
        uint8_t Reserved[3];
    } __attribute__((packed));

    struct SDTHeader
    {
        unsigned char Signature[4];
        uint32_t Length;
        uint8_t Revision;
        uint8_t Checksum;
        uint8_t OEMID[6];
        uint8_t OEMTableID[8];
        uint32_t OEMRevision;
        uint32_t CreatorID;
        uint32_t CreatorRevision;
    } __attribute__((packed));
    

    struct MCFGHeader
    {
        SDTHeader Header;
        uint64_t Reserved;
    } __attribute__((packed));

    struct MADTRecordHeader {
        uint8_t Type;
        uint8_t Length;
    } __attribute__((packed));

    struct MADTRecord{
        MADTRecordHeader Header;
        uint8_t Data[];
    } __attribute__((packed));

    struct MADTLocalApicRecord {
        MADTRecordHeader Header;
        uint8_t ProcessorID;
        uint8_t ApicID;
        uint32_t Flags;
    } __attribute__((packed));

    struct MADTHeader
    {
        SDTHeader Header;
        uint32_t LocalAPICAddress;
        uint32_t Flags;
    } __attribute__((packed));
    
    struct MADTTable {
        MADTHeader Header;
        MADTRecord Records[];
    } __attribute__((packed));

    struct DeviceConfig
    {
        uint64_t BaseAddress;
        uint16_t PCISegGroup;
        uint8_t StartBus;
        uint8_t EndBus;
        uint32_t Reserved;
    } __attribute__((packed));

    void* FindTable(SDTHeader* sdtHeader, char* signature);

#define PrintSignature(header, sig_field, size, func, args) for (int _; _ < size; _++) func (args, header->sig_field [_] )
}