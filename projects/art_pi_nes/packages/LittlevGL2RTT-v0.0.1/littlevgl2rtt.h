#ifndef __LITTLEVGL2RTT_H__
#define __LITTLEVGL2RTT_H__

#include <rtthread.h> 
#include <rtdevice.h> 

enum littlevgl2rtt_input_state
{
    LITTLEVGL2RTT_INPUT_NONE = 0x00,
    LITTLEVGL2RTT_INPUT_UP   = 0x01, 
    LITTLEVGL2RTT_INPUT_DOWN = 0x02, 
    LITTLEVGL2RTT_INPUT_MOVE = 0x03
}; 

rt_err_t littlevgl2rtt_init(const char *name); 
void littlevgl2rtt_send_input_event(rt_int16_t x, rt_int16_t y, rt_uint8_t state); 
void littlevgl2rtt_deinit(void);

#endif
