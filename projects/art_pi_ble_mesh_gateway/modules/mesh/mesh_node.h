#ifndef MESH_NODE_H
#define MESH_NODE_H

#include <stdbool.h>
#include <stdint.h>
#include "mesh_command.h"
#include "cJSON.h"

bool mesh_node_add(mesh_node_t node);
cJSON *mesh_nodes_dump();
int16_t mesh_node_find(uint16_t addr);
mesh_node_t mesh_node_get(uint16_t index);
void mesh_node_update_data(uint16_t index, uint8_t *data);

#endif