/*
 * File      : stack.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-7-14      Peng Fan     sep6200 implementation
 */

#include <rtthread.h>
#include <sep6200.h>

/**
 * @addtogroup sep6200
 */
/*@{*/

/**
 * This function will initialize thread stack
 *
 * @param tentry the entry of thread
 * @param parameter the parameter of entry
 * @param stack_addr the beginning stack address
 * @param texit the function will be called when thread exit
 *
 * @return stack address
 */

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
	rt_uint8_t *stack_addr, void *texit)
{
	rt_uint32_t *stk;

	stk 	 = (rt_uint32_t*)stack_addr;
	*(stk) 	 = (rt_uint32_t)tentry;			/* entry point */
	*(--stk) = (rt_uint32_t)texit;			/* lr */
	*(--stk) = 0;							/* r28 */
	*(--stk) = 0;							/* r27 */
	*(--stk) = 0;							/* r26 */
	*(--stk) = 0;							/* r25 */
	*(--stk) = 0;							/* r24 */
	*(--stk) = 0;							/* r23 */
	*(--stk) = 0;							/* r22 */
	*(--stk) = 0;							/* r21 */
	*(--stk) = 0;							/* r20 */
	*(--stk) = 0;							/* r19 */
	*(--stk) = 0;							/* r18 */
	*(--stk) = 0;							/* r17 */
	*(--stk) = 0;							/* r16 */
	*(--stk) = 0;							/* r15 */
	*(--stk) = 0;							/* r14 */
	*(--stk) = 0;							/* r13 */
	*(--stk) = 0;							/* r12 */
	*(--stk) = 0;							/* r11 */
	*(--stk) = 0;							/* r10 */
	*(--stk) = 0;							/* r9 */
	*(--stk) = 0;							/* r8 */
	*(--stk) = 0;							/* r7 */
	*(--stk) = 0;							/* r6 */
	*(--stk) = 0;							/* r5 */
	*(--stk) = 0;							/* r4 */
	*(--stk) = 0;							/* r3 */
	*(--stk) = 0;							/* r2 */
	*(--stk) = 0;							/* r1 */
	*(--stk) = (rt_uint32_t)parameter; /* r0 : argument */
	*(--stk) = Mode_PRIV;					/* asr */
	*(--stk) = Mode_PRIV;					/* bsr */ /*why both PRIV do not need switch?*/

	/* return task's current stack address */
	return (rt_uint8_t *)stk;
}

/*@}*/
