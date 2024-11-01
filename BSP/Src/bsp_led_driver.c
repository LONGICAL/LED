/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_led_driver.c
 *
 * @par dependencies
 * - bsp_led_driver.h
 * -
 * -
 *
 * @author Long | R&D Dept. | EternalChip
 *
 * @brief Provide the HAL APIs of LED and corresponding opetions.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2024-10-28
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/

//******************************** Includes *********************************//

#include "bsp_led_driver.h"

//******************************** Includes *********************************//
//******************************** Defines **********************************//

/**
 * @brief perform the detailed of the bsp_led_driver_t targrt.
 *
 * Steps:
 * 1. analyze the target paras..
 * 2. do the appropriate operation of led.
 *
 * @param[in]
 *
 * @return led_status_t : The status of running.
 **/
led_status_t led_blink(bsp_led_driver_t *self)
{
    led_status_t ret = LED_OK;
    // 1.check if the target has been initialized.
    if (NULL == self || NOT_INITED == self->is_inited)
    {
        // if not instantiated, return error to caller.
        // TBD:option - mutex to upgrade low priority task to init target ASP.
#ifdef DEBUG
        ret = LED_ERRORPARAMETER;
#endif //DEBUG
    }
    //2.analyze the features.
    {
        //2.1 define the value.
        uint32_t        cycle_time_local;
        uint32_t        blink_time_local;
        proportion_t    proportion_local;
        uint32_t         led_toggle_time;

        cycle_time_local = self->     cycle_time_ms;
        blink_time_local = self->       blink_times;
        proportion_local = self-> porportion_on_off;
        //2.2 caculate the led toggle time.
        if(PROPORTION_1_1 == proportion_local)
        {
            led_toggle_time = cycle_time_local;
        }
        else if(PROPORTION_1_2 == proportion_local)
        {
            led_toggle_time = cycle_time_local / 2;
        }
        else if(PROPORTION_1_3 == proportion_local)
        {
            led_toggle_time = cycle_time_local / 3;
        }
        else
        {
#ifdef DEBUG
            DEBUG_OUT("LED_ERRORPARAMETER in led_blink/r/n");
            
#endif //DEBUG
        return LED_ERRORPARAMETER;
        }
        //2.3 do the operation of led.
        //TBD:4.option - use mutex to protect the critical section.
        for(uint32_t i = 0; i < blink_time_local; i++)
        {
                //TBD:5.option - use mutex to protect the critical section.
                for(uint32_t j = 0; j < cycle_time_local; j++)
                {
                    //TBD:6.option - use mutex to protect the critical section.
                    if (j < led_toggle_time)
                    {
                        //TBD:7.option - LED_on.
                        self->p_led_ops_inst->pf_led_on;
                    }
                    else
                    {
                        //TBD:8.option - LED_off.
                        self->p_led_ops_inst->pf_led_off;
                    }
                }
        }
    }
    return ret;
}


#ifndef OS_SUPPORT
/**
 * @brief 
 *
 * Steps:
 * 1. 
 * 2.
 * 3. 
 * 4.
 * @param[in]
 *
 * @return 
 **/
static led_status_t led_control (
                              bsp_led_driver_t *const self,
                              uint32_t cycle_time,      /* cycle_time[ms]    */
                              uint32_t blink_time,      /* blink_times[times]*/
                              proportion_t proportion)  /* portportion_on_off*/
{
    /********** 1.Checking the target status *************/
    led_status_t ret = LED_OK;
    // 1.check if the target has been initialized.
    if( NULL == self || NOT_INITED == self->is_inited )
    {
        // 2.if not instantiated, return error to caller.
        //TBD:3.option - mutex to upgrade low priority task to init target ASP.
        ret = LED_ERRORPARAMETER;
        return ret;

    }

    /********** 2.Checking the input parameter ***********/
    //cycle_time checking
    if(!(cycle_time < 10000                   &&
         blink_time < 1000                    &&
        ((0 < proportion) && (3 < proportion))))
    {
        ret = LED_ERRORPARAMETER;
        return ret;
    }

    /********** 3.Adding the data in target **************/
    self->cycle_time_ms     = cycle_time;
    self->blink_times       = blink_time;
    self->porportion_on_off = proportion;

    /********** 4.Run the operation of LED ***************/
    //1.call the blink of LED.
    ret = led_blink(self);
    return ret;
}
#endif // OS_SUPPORT

/**
 * @brief Init the target of bsp_led_driver_t.
 *
 * Steps:
 * 1. make the target at a specific status.
 *
 * @param[in] self Pointer to the input data.
 *
 * @return led_status_t : The status of running.
 **/
led_status_t led_driver_init(bsp_led_driver_t *const self)
{
    led_status_t ret = LED_OK;
    DEBUG_OUT("led init start \r\n");
    if (NULL == self)
    {
#ifdef DEBUG
        DEBUG_OUT("LED_ERRORSOURCE\r\n");
        return LED_ERRORSOURCE;
#endif // DEBUG
        return LED_ERRORPARAMETER;
    }

    self->p_led_ops_inst->pf_led_off();
    self->p_os_time_delay->pf_os_delay_ms(600);
    uint32_t time_stamp = 0;
    self->p_time_base_ms->pf_get_time_ms(&time_stamp);

    return ret;
}

/**
 * @brief Instantiate the target of bsp_led_driver_t.
 *
 * Steps:
 * 1. Validate input parameters for null pointers
 * 2. Initialize the LED driver structure with provided operations
 * 3. Configure time base and delay functions if OS support is enabled
 *
 * @param[in] self Pointer to the LED driver instance to be initialized
 * @param[in] led_ops Pointer to LED operations structure containing function
 *                    pointers
 * @param[in] time_base_ms Pointer to time base configuration (OS mode only)
 * @param[in] os_time_delay Pointer to OS delay function implementation
 *
 * @return led_status_t : LED_STATUS_OK if initialization successful
 *                        LED_STATUS_INVALID_PARAM if any input pointer is NULL
 *                        LED_STATUS_ERROR if initialization fails
 **/
led_status_t led_driver_inst(
    bsp_led_driver_t *const self,
    const led_operations_t *const led_ops,
#ifdef OS_SUPPORTING
    const time_base_ms_t *const os_delay,
#endif // OS_SUPPORTING
    const os_delay_t *const time_base)

{
    led_status_t ret;
#ifdef DEBUG
    DEBUG_OUT("led inst kick-off\r\n");
#endif // DEBUG

    /***** 1.Checking the input parameters *****/
    if (NULL == self ||
        NULL == led_ops ||
        NULL == os_delay ||
        NULL == time_base)
        if (INITED == self->is_inited)
        {
#ifdef DEBUG
            DEBUG_OUT("LED_ERRORSOURCE\r\n");   
#endif // DEBUG 
            return LED_ERRORSOURCE;
        }
    /******** 2.Checking the Resources ********/
    if( INITED == self->is_inited )
    {
#ifdef DEBUG
        DEBUG_OUT("LED_ERRORSOURCE\r\n");
        return LED_ERRORSOURCE;
#endif // DEBUG
        
    }
#ifdef DEBUG
    DEBUG_OUT("led_inst_start\r\n");
#endif // DEBUG

    /******** 3.Adding the interface **********/

    /**** 3.1 mount external interface ****/
    self->p_led_ops_inst  =   led_ops;
    self->p_os_time_delay =  os_delay;
    self->p_time_base_ms  = time_base;

    /**** 3.2 mount internal interface ****/
#ifdef OS_SUPPORTING
    self->pf_led_controler = led_control;
#endif // OS_SUPPORTING

    /******** 4.Init the target ***********/
    self->blink_times       =              0;
    self->cycle_time_ms     =              0;
    self->porportion_on_off = PROPORTION_x_x;

    ret = led_driver_init(self);
    if( LED_OK != ret )
    {
#ifdef DEBUG
        DEBUG_OUT("LED init failed\r\n");
#endif // DEBUG
        self->p_led_ops_inst  = NULL;
        self->p_os_time_delay = NULL;
        self->p_time_base_ms  = NULL;
        return ret;
    }
    self->is_inited = INITED;
#ifdef DEBUG
    DEBUG_OUT("led inst finished\r\n");
#endif // DEBUG
    return ret;
}
//******************************** Defines **********************************//

//******************************** Declaring ********************************//

//******************************** Declaring ********************************//