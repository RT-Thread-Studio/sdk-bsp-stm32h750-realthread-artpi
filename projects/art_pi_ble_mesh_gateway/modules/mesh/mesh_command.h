#ifndef MESH_COMMAND_H
#define MESH_COMMAND_H

#define COMMAND_DATA_LENGTH 8

typedef enum
{
    COMMAND_OPCODE_FIND = 0xC1,     // The gateway find device.
    COMMAND_OPCODE_REGISTER = 0xC2, // The device register.
    COMMAND_OPCODE_SET = 0xC3,      // Set device.
    COMMAND_OPCODE_STATUS = 0xC4,   // Device status.
    COMMAND_OPCODE_GET = 0xC5,      // Get device data.
    COMMAND_OPCODE_DATA = 0xC6      // Device response data.
} command_opcode_t;

typedef enum
{
    COMMAND_SERVER_LIGHT = 1,
    COMMAND_SERVER_SWITCH = 2,
    COMMAND_SERVER_COUNTER = 3
} mesh_node_type_t;

typedef struct
{
    uint16_t addr;
    mesh_node_type_t type;
    uint8_t data[COMMAND_DATA_LENGTH];
} mesh_node_t;

#endif