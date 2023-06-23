#include "Debugger.h"

static SerialPort DebugSerialLine = (SerialPort)0;

bool GDB::IsValidPackage(RawPacket packet)
{
    return (
        packet.as_cstr() != nullptr &&
        packet.size() >= 2            &&
        packet[0] == '$'            &&
        packet.containes('#')
    );
}

void GDB::ParsePacket(RawPacket packet)
{
    if (!IsValidPackage(packet)) {
        Serial::WriteFormat(COM1, "[GDB Remote Debugging Protocol] Tried to parse invalid Packet `%s`!\r\n", packet.as_cstr());
        return;
    }

    std::string_view packet_data(packet.as_cstr(), packet.size());

    (packet_data.buffer())++; // Exclude the starting `$`.
    (packet_data.size())--; // Ajust size!
    
    Serial::WriteFormat(COM1, "[GDB Remote Debugging Protocol] Packet Data: `%s`", packet_data.buffer());

}

ANTSTATUS KeInitializeDebugger(SerialPort debugLine)
{
    if (DebugSerialLine != (SerialPort)0) return STATUS_ALREADY_COMPLETE;

    if (!Serial::InitPort(debugLine)) return STATUS_UNSUCCESSFUL;

    DebugSerialLine = debugLine;

    return STATUS_SUCCESS;
}
