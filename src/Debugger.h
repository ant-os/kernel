#pragma once
#include <ant/string.hpp>
#include "Serial.h"
#include "antstatus.h"

/// @brief An implemenation of the GDB Remote Debugging Protocol!
namespace GDB{
    
    /// @brief The raw serial line of a Packet!
    typedef std::string RawPacket;
    bool IsValidPackage(RawPacket packet);
    void ParsePacket(RawPacket packet);
}

/// @brief Initializes the GDB Remote Debugging Protocol for a given Serial Port!
/// @note This function can only be called once! If already called STATUS_ALREADY_COMPLETE will be returned!
/// @return Returns STATUS_SUCCESS if the Debugger was initiualized sucessfully, adnd STATUS_UNSUCCESSFUL if the Port is faulty!
ANTSTATUS KeInitializeDebugger(SerialPort debugLine);