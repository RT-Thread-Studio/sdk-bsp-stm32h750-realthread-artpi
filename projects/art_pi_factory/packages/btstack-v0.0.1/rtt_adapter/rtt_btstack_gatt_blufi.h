
// binary attribute representation:
// - size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#include <stdint.h>

const uint8_t profile_data[] =
{
    // ATT DB Version
    1,

    // 0x0001 PRIMARY_SERVICE-FFFF
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0xff, 0xff, 
    // 0x0002 CHARACTERISTIC-FF01-READ | WRITE | NOTIFY | DYNAMIC
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x1a, 0x03, 0x00, 0x01, 0xff,
    // 0x0003 VALUE-FF01-READ | WRITE | NOTIFY | DYNAMIC-''
    // READ_ANYBODY, WRITE_ANYBODY
    0x08, 0x00, 0x0a, 0x01, 0x03, 0x00, 0x01, 0xff, 
    // 0x0004 CLIENT_CHARACTERISTIC_CONFIGURATION
    // READ_ANYBODY, WRITE_ANYBODY
    0x0a, 0x00, 0x0e, 0x01, 0x04, 0x00, 0x02, 0x29, 0x00, 0x00,

    // END
    0x00, 0x00, 
}; // total size 25 bytes


//
// list service handle ranges
//
#define ATT_SERVICE_FFFF_START_HANDLE 0x0001
#define ATT_SERVICE_FFFF_END_HANDLE 0x0004

//
// list mapping between characteristics and handles
//
#define ATT_CHARACTERISTIC_FF01_01_VALUE_HANDLE 0x0003
#define ATT_CHARACTERISTIC_FF01_01_CLIENT_CONFIGURATION_HANDLE 0x0004

