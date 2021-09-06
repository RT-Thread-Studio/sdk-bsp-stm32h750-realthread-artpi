
// le_data_channel_server.h generated from ../../example/le_data_channel_server.gatt for BTstack
// it needs to be regenerated when the .gatt file is updated. 

// To generate le_data_channel_server.h:
// ../../tool/compile_gatt.py ../../example/le_data_channel_server.gatt le_data_channel_server.h

// att db format version 1

// binary attribute representation:
// - size in bytes (16), flags(16), handle (16), uuid (16/128), value(...)

#include <stdint.h>

const uint8_t profile_data[] =
{
    // ATT DB Version
    1,

    // 0x0001 PRIMARY_SERVICE-GAP_SERVICE
    0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x18, 
    // 0x0002 CHARACTERISTIC-GAP_DEVICE_NAME-READ
    0x0d, 0x00, 0x02, 0x00, 0x02, 0x00, 0x03, 0x28, 0x02, 0x03, 0x00, 0x00, 0x2a, 
    // 0x0003 VALUE-GAP_DEVICE_NAME-READ-'LE Data Channel'
    // READ_ANYBODY
    0x17, 0x00, 0x02, 0x00, 0x03, 0x00, 0x00, 0x2a, 0x4c, 0x45, 0x20, 0x44, 0x61, 0x74, 0x61, 0x20, 0x43, 0x68, 0x61, 0x6e, 0x6e, 0x65, 0x6c, 

    // END
    0x00, 0x00, 
}; // total size 25 bytes 


//
// list service handle ranges
//
#define ATT_SERVICE_GAP_SERVICE_START_HANDLE 0x0001
#define ATT_SERVICE_GAP_SERVICE_END_HANDLE 0x0003

//
// list mapping between characteristics and handles
//
#define ATT_CHARACTERISTIC_GAP_DEVICE_NAME_01_VALUE_HANDLE 0x0003
