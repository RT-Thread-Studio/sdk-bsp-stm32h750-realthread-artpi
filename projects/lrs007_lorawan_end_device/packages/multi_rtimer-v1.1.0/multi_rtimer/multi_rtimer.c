/*!
 * \file      multi_rtimer.c
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
#include "board.h"
#ifdef MULTI_RTIMER_USING_TRAGET_MCU_STM32_RTC
#include "hw_rtc_stm32.h"
#endif
#include "multi_rtimer.h"

/*!
 * Safely execute call back
 */
#define EXECUTE_CALLBACK( _callback_ )         \
    do                                         \
    {                                          \
        if( _callback_ == NULL )               \
        {                                      \
            while( 1 );                        \
        }                                      \
        else                                   \
        {                                      \
            _callback_( );                     \
        }                                      \
    }while( 0 );

/*!
 * Timers list head pointer
 */
static timer_event_t *timer_list_head = NULL;

/*!
 * \brief Sets a timeout with the duration "timestamp"
 *
 * \param [IN] timestamp Delay duration
 */
static void rtimer_set_timeout( timer_event_t *obj );

/*!
 * \brief Check if the Object to be added is not already in the list
 *
 * \param [IN] timestamp Delay duration
 * \retval true (the object is already in the list) or false
 */
static bool rtimer_exists( timer_event_t *obj );

void rtimer_init( timer_event_t *obj, void ( *callback )( void ) )
{
    obj->Timestamp = 0;
    obj->ReloadValue = 0;
    obj->IsStarted = false;
    obj->IsNext2Expire = false;
    obj->Callback = callback;
    obj->Context = NULL;
    obj->Next = NULL;
}

static void rtimer_reload(void)
{
    // Start the next timer_list_head if it exists AND NOT running
    if( ( timer_list_head != NULL ) && ( timer_list_head->IsNext2Expire == false ) )
    {
        rtimer_set_timeout( timer_list_head );
    }
}

void rtimer_start( timer_event_t *obj )
{
    uint32_t elapsedTime = 0;
    timer_event_t **target = &timer_list_head;

    MULTI_RTIMER_CRITICAL_SECTION_BEGIN( );

    if( ( obj == NULL ) || ( rtimer_exists( obj ) == true ) )
    {
        MULTI_RTIMER_CRITICAL_SECTION_END( );
        return;
    }

    obj->Timestamp = obj->ReloadValue;
    obj->IsStarted = true;
    obj->IsNext2Expire = false;
    obj->Next = NULL;
    
    // The timer list is automatically sorted. The timer list head always contains the next timer to expire.
    if( timer_list_head == NULL )
    {
        rtc_set_timer_context( );
    }
    else
    {   
        elapsedTime = rtc_get_timer_elapsed_time( );
        obj->Timestamp += elapsedTime;
        
        for(; *target; target = &((*target)->Next) )
        {
            if(((*target)->Timestamp > obj->Timestamp))
            {
                (*target)->IsNext2Expire = false;
                
                obj->Next = *target;
                
                break;
            }
        }
    }
    *target = obj; 
    
    // Start the next timer_list_head if it exists AND NOT running
    rtimer_reload();
    
    MULTI_RTIMER_CRITICAL_SECTION_END( );
}

void rtimer_stop( timer_event_t *obj )
{
    MULTI_RTIMER_CRITICAL_SECTION_BEGIN( );
  
    timer_event_t **target = &timer_list_head;
    
    // List is empty or the obj to stop does not exist
    if( ( timer_list_head == NULL ) || ( obj == NULL ) )
    {
        MULTI_RTIMER_CRITICAL_SECTION_END( );
        return;
    }
    
    obj->IsStarted = false;
    
    // Stop the Head and RTC Timeout
    if( timer_list_head == obj ) 
    {
        // The head is already running
        if( timer_list_head->IsNext2Expire == true ) 
        {
            timer_list_head->IsNext2Expire = false;
            if( timer_list_head->Next != NULL )
            {
                timer_list_head = timer_list_head->Next;
                rtimer_set_timeout( timer_list_head );
            }
            else
            {
                rtc_stop_alarm( );
                timer_list_head = NULL;
            }
                                        
            MULTI_RTIMER_CRITICAL_SECTION_END( );
            return;
        }
    }
    
    // Stop an object within the list
    for(; *target; target = &((*target)->Next) )
    {
        if( *target == obj )
        {
            *target = obj->Next;
            break;
        }
    }
    
    MULTI_RTIMER_CRITICAL_SECTION_END( );
}

static bool rtimer_exists( timer_event_t *obj )
{
    timer_event_t* cur = timer_list_head;

    while( cur != NULL )
    {
        if( cur == obj )
        {
            return true;
        }
        cur = cur->Next;
    }
    return false;
}

void rtimer_reset( timer_event_t *obj )
{
    rtimer_stop( obj );
    rtimer_start( obj );
}

void rtimer_set_value( timer_event_t *obj, uint32_t value )
{
    uint32_t minValue = 0;
    uint32_t ticks = rtc_ms2tick( value );

    rtimer_stop( obj );

    minValue = rtc_get_minimum_timeout( );

    if( ticks < minValue )
    {
        ticks = minValue;
    }

    obj->Timestamp = ticks;
    obj->ReloadValue = ticks;
}

TimerTime_t rtimer_get_current_time( void )
{
    uint32_t now = rtc_get_timer_value( );
    return  rtc_tick2ms( now );
}

TimerTime_t rtimer_get_elapsed_time( TimerTime_t past )
{
    if ( past == 0 )
    {
        return 0;
    }
    uint32_t nowInTicks = rtc_get_timer_value( );
    uint32_t pastInTicks = rtc_ms2tick( past );

    // Intentional wrap around. Works Ok if tick duration below 1ms
    return rtc_ms2tick( nowInTicks - pastInTicks );
}

static void rtimer_set_timeout( timer_event_t *obj )
{
    int32_t minTicks= rtc_get_minimum_timeout( );
    obj->IsNext2Expire = true;

    // In case deadline too soon
    if( obj->Timestamp  < ( rtc_get_timer_elapsed_time( ) + minTicks ) )
    {
        obj->Timestamp = rtc_get_timer_elapsed_time( ) + minTicks;
    }
    rtc_set_alarm( obj->Timestamp );
}

void rtimer_irq_handler( void )
{
    timer_event_t** target = &timer_list_head;
    timer_event_t* cur;

    uint32_t old =  rtc_get_timer_context( );
    uint32_t now =  rtc_set_timer_context( );
    // intentional wrap around
    uint32_t deltaContext = now - old; 

    // Update timeStamp based upon new Time Reference
    // because delta context should never exceed 2^32
    if( timer_list_head != NULL )
    {
        for(; (*target)->Next; target = &((*target)->Next) )
        {
            if( (*target)->Next->Timestamp > deltaContext )
            {
                (*target)->Next->Timestamp -= deltaContext;
            }
            else
            {
                (*target)->Next->Timestamp = 0;
            }
        }

        // Execute immediately the alarm callback
        cur = timer_list_head;
        timer_list_head = timer_list_head->Next;
        cur->IsStarted = false;
        EXECUTE_CALLBACK( cur->Callback );
    }

    // Remove all the expired object from the list
    while( ( timer_list_head != NULL ) && ( timer_list_head->Timestamp < rtc_get_timer_elapsed_time( ) ) )
    {
        cur = timer_list_head;
        timer_list_head = timer_list_head->Next;
        cur->IsStarted = false;
        EXECUTE_CALLBACK( cur->Callback );
    }

    // Start the next timer_list_head if it exists AND NOT running
    rtimer_reload();
}

#ifdef MULTI_RTIMER_USING_RTC_TEMPERTURE_COMPENSATION
TimerTime_t rtimer_temp_compensation( TimerTime_t period, float temperature )
{
    return rtc_temp_compensation( period, temperature );
}
#endif

void rtimer_process( void )
{
    rtc_process( );
}
