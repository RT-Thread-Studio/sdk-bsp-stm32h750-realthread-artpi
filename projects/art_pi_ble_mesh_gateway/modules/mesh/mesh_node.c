#include <stdio.h>
#include "rtthread.h"
#include "mesh_node.h"
#include "hex_utils.h"

static mesh_node_t *mesh_nodes = NULL;
static uint16_t node_num = 0;
static rt_mutex_t write_mutex = NULL;

bool mesh_node_add(mesh_node_t node)
{
    if (write_mutex == NULL)
    {
        write_mutex = rt_mutex_create("write_mutex", RT_IPC_FLAG_FIFO);
    }

    if (mesh_node_find(node.addr) != -1)
    {
        return false;
    }

    rt_mutex_take(write_mutex, RT_WAITING_FOREVER);
    if (mesh_nodes == NULL)
    {
        mesh_nodes = rt_malloc(sizeof(mesh_node_t));
    }
    else
    {
        mesh_nodes = rt_realloc(mesh_nodes, node_num + 1 * sizeof(mesh_node_t));
    }

    mesh_nodes[node_num] = node;
    node_num += 1;

    rt_mutex_release(write_mutex);

    return true;
}

cJSON *mesh_nodes_dump()
{
    cJSON *root = cJSON_CreateObject();
    for (uint16_t i = 0; i < node_num; i++)
    {
        mesh_node_t node = mesh_nodes[i];

        char node_data[COMMAND_DATA_LENGTH * 2];

        for (uint8_t i = 0; i < sizeof(node.data); i++)
        {
            uint8_to_hex(node.data[i], &node_data[i * 2]);
        }

        cJSON *temp = cJSON_CreateObject();
        cJSON_AddItemToObject(temp, "type", cJSON_CreateNumber(node.type));
        cJSON_AddItemToObject(temp, "data", cJSON_CreateString(node_data));

        char node_addr[5];

        sprintf(node_addr, "%04X", node.addr);

        cJSON_AddItemToObject(root, node_addr, temp);
    }

    return root;
}

int16_t mesh_node_find(uint16_t addr)
{
    for (uint16_t i = 0; i < node_num; i++)
    {
        if (addr == mesh_nodes[i].addr)
            return i;
    }

    return -1;
}

mesh_node_t mesh_node_get(uint16_t index)
{
    return mesh_nodes[index];
}

void mesh_node_update_data(uint16_t index, uint8_t *data)
{
    if (write_mutex == NULL)
    {
        write_mutex = rt_mutex_create("write_mutex", RT_IPC_FLAG_FIFO);
    }

    rt_mutex_take(write_mutex, RT_WAITING_FOREVER);

    mesh_node_t *node = &mesh_nodes[index];

    rt_memcpy(node->data, data, sizeof(data));

    rt_mutex_release(write_mutex);
}