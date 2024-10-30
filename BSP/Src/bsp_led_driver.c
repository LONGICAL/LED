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
#ifdef DEBUG
    DEBUG_OUT("led inst start\r\n");
#endif // DEBUG
    led_status_t ret;
    /***** 1.Checking the input parameters *****/

    if (NULL == self ||
        NULL == led_ops ||
        NULL == os_delay ||
        NULL == time_base)
        if (INITED == self->is_inited)
        {
#ifdef DEBUG
            DEBUG_OUT("LED_ERRORSOURCE\r\n");
            return LED_ERRORSOURCE;
#endif // DEBUG
        }
/******** 2.Checking the Resources ********/
#ifdef DEBUG
    DEBUG_OUT("led_inst_start\r\n");
#endif // DEBUG

    /******** 3.Adding the interface **********/

    /**** 3.1 mount external interface ****/
    self->p_led_ops_inst = led_ops;
    self->p_os_time_delay = os_delay;
    self->p_time_base_ms = time_base;

    /**** 3.2 mount internal interface ****/
    self->blink_times = 0;
    self->cycle_time_ms = 0;
    self->porportion_on_off = PROPORTION_x_x;
}
//******************************** Defines **********************************//

//******************************** Declaring ********************************//

//******************************** Declaring ********************************//