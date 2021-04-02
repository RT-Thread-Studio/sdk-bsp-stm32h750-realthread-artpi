/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-10     xqyjlj       the first version
 */
#include "ld3320.h"

#ifdef PKG_USING_LD3320_FINSH
static ld3320_t ld3320_head;

void ld3320_finsh_init(ld3320_t ops)
{
    ld3320_head=ops;
}
static void ld3320_list_command_fromnode(ld3320_t ops)
{
    ld3320_command_t tmp;
    rt_list_t *node;
    node = ops->node.next;
    for (; node != &(ops->node); node = node->next) {
        tmp = rt_list_entry(node, struct ld3320_command, list);
        rt_kprintf("%s<-->%d\n", tmp->name, tmp->num);
    }
}

static void ld3320(int argc, char *argv[])
{
    ld3320_list_command_fromnode(ld3320_head);
}

MSH_CMD_EXPORT(ld3320, list ld3320 command);
#endif


