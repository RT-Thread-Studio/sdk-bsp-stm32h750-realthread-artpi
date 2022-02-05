/**
 * @file lv_demo_rtt_music_list.h
 *
 */

#ifndef LV_DEMO_RTT_MUSIC_LIST_H
#define LV_DEMO_RTT_MUSIC_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lv_demo_rtt_music.h"
#if LV_USE_DEMO_RTT_MUSIC

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
lv_obj_t * _lv_demo_music_list_create(lv_obj_t * parent);
void _lv_demo_music_list_btn_check(uint32_t track_id, bool state);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_DEMO_RTT_MUSIC*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_RTT_MUSIC_LIST_H*/
