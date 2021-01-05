/*!
 * \file      multi_rtimer.h
 *
 * \brief     Timer objects and scheduling management implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
  *
 * \author    Forest-Rain
 */
#ifndef __RTIMER_H__
#define __RTIMER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/*!
 * Begins critical section
 */
#define MULTI_RTIMER_CRITICAL_SECTION_BEGIN( ) register rt_base_t level; level = rt_hw_interrupt_disable()

/*!
 * Ends critical section
 */
#define MULTI_RTIMER_CRITICAL_SECTION_END( ) rt_hw_interrupt_enable(level)

    
#if 1 
/*!
 * LoRaWAN Timer API
 */
#define TimerInit            rtimer_init
#define TimerStart           rtimer_start
#define TimerStop            rtimer_stop   
#define TimerReset           rtimer_reset
#define TimerSetValue        rtimer_set_value
#define TimerGetCurrentTime  rtimer_get_current_time
#define TimerGetElapsedTime  rtimer_get_elapsed_time
#define TimerEvent_t         timer_event_t
#endif    
    
/*!
 * \brief Timer object description
 */
typedef struct TimerEvent_s
{
    uint32_t Timestamp;                  //! Current timer value
    uint32_t ReloadValue;                //! Timer delay value
    bool IsStarted;                      //! Is the timer currently running
    bool IsNext2Expire;                  //! Is the next timer to expire
    void ( *Callback )( void );          //! Timer IRQ callback function
    void *Context;                       //! User defined data object pointer to pass back
    struct TimerEvent_s *Next;           //! Pointer to the next Timer object.
}timer_event_t;

/*!
 * \brief Timer time variable definition
 */
#ifndef TimerTime_t
typedef uint32_t TimerTime_t;
#define TIMERTIME_T_MAX                             ( ( uint32_t )~0 )
#endif

/*!
 * \brief Initializes the timer object
 *
 * \remark rtimer_set_value function must be called before starting the timer.
 *         this function initializes timestamp and reload value at 0.
 *
 * \param [IN] obj          Structure containing the timer object parameters
 * \param [IN] callback     Function callback called at the end of the timeout
 */
void rtimer_init( timer_event_t *obj, void ( *callback )( void ) );

/*!
 * \brief Starts and adds the timer object to the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void rtimer_start( timer_event_t *obj );

/*!
 * \brief Stops and removes the timer object from the list of timer events
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void rtimer_stop( timer_event_t *obj );

/*!
 * \brief Resets the timer object
 *
 * \param [IN] obj Structure containing the timer object parameters
 */
void rtimer_reset( timer_event_t *obj );

/*!
 * \brief Set timer new timeout value
 *
 * \param [IN] obj   Structure containing the timer object parameters
 * \param [IN] value New timer timeout value
 */
void rtimer_set_value( timer_event_t *obj, uint32_t value );

/*!
 * \brief Read the current time
 *
 * \retval time returns current time
 */
TimerTime_t rtimer_get_current_time( void );

/*!
 * \brief Return the Time elapsed since a fix moment in Time
 *
 * \remark TimerGetElapsedTime will return 0 for argument 0.
 *
 * \param [IN] past         fix moment in Time
 * \retval time             returns elapsed time
 */
TimerTime_t rtimer_get_elapsed_time( TimerTime_t past );

/*!
 * \brief Computes the temperature compensation for a period of time on a
 *        specific temperature.
 *
 * \param [IN] period Time period to compensate
 * \param [IN] temperature Current temperature
 *
 * \retval Compensated time period
 */
TimerTime_t rtimer_temp_compensation( TimerTime_t period, float temperature );

/*!
 * Timer IRQ event handler
 */
void rtimer_irq_handler( void );

/*!
 * \brief Processes pending timer events
 */
void rtimer_process( void );
#ifdef __cplusplus
}
#endif

#endif // __RTIMER_H__

