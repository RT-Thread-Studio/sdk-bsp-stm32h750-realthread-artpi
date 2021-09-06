/*
 * qled.h
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-06-22     qiyongzhong       first version
 */

#ifndef __QLED_H__
#define __QLED_H__

#include <typedef.h>
#include <rtconfig.h>

#ifndef QLED_TOTAL
#define QLED_TOTAL               8       //support led maximum total
#endif

#ifndef QLED_TIME_UNIT_MS
#define QLED_TIME_UNIT_MS        10      //led blink time unit
#endif

#ifndef QLED_THREAD_NAME
#define QLED_THREAD_NAME         "qled"   //led driver thread name
#endif

#ifndef QLED_THREAD_STACK_SIZE
#define QLED_THREAD_STACK_SIZE   1024    //led driver thread stack size
#endif

#ifndef QLED_THREAD_PRIO
#define QLED_THREAD_PRIO         2       //led driver thread priority
#endif

/* 
 * @brief   add led to driver
 * @param   pin      - led control pin
 * @param   level    - led on level
 * @retval  0 - success, other - error
 */
int qled_add(int pin, int level);

/* 
 * @brief   remove led from driver
 * @param   pin      - led control pin
 * @retval  none
 */
void qled_remove(int pin);

/* 
 * @brief   set led off alway 
 * @param   pin      - led control pin
 * @retval  0 - success, other - error
 */
int qled_set_off(int pin);

/* 
 * @brief   set led on alway
 * @param   pin      - led control pin
 * @retval  0 - success, other - error
 */
int qled_set_on(int pin);

/* 
 * @brief   set led periodic blink 
 * @param   pin      - led control pin
 * @param   ton_ms   - led on time, unit : ms
 * @param   toff_ms  - led off time, unit : ms
 * @retval  0 - success, other - error
 */
int qled_set_blink(int pin, int ton_ms, int toff_ms);

/* 
 * @brief   set led run special sequence 
 * @param   pin         - led control pin
 * @param   datas       - led on/off time datas, unit : ms
 * @param   data_total  - data total
 * @param   over_cb     - led run special sequence over callback function
 * @retval  0 - success, other - error
 */
int qled_set_special(int pin, const u16 *datas, int data_total, void (*over_cb)(void));

#endif

