#ifndef MESH_COMMAND_H
#define MESH_COMMAND_H

#define COMMAND_DATA_LENGTH 8

typedef enum
{
    COMMAND_OPCODE_FIND = 0xC1,     // 网关寻找设备
    COMMAND_OPCODE_REGISTER = 0xC2, // 设备向网关提交注册
    COMMAND_OPCODE_SET = 0xC3,      // 设置设备
    COMMAND_OPCODE_STATUS = 0xC4,   // 设备状态
    COMMAND_OPCODE_GET = 0xC5,      // 读取设备
    COMMAND_OPCODE_DATA = 0xC6      // 设备数据
} command_opcode_t;

typedef enum
{
    COMMAND_SERVER_LIGHT = 1,  // 灯
    COMMAND_SERVER_SWITCH = 2, // 开关
    COMMAND_SERVER_COUNTER = 3 // 计数器
} mesh_node_type_t;

typedef struct
{
    uint16_t addr;
    mesh_node_type_t type;
    uint8_t data[COMMAND_DATA_LENGTH];
} mesh_node_t;

#endif