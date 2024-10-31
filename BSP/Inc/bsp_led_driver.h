/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_LED_driver.h
 *
 * @par dependencies
 * - bsp_LED_driver.h
 * - stdio.h
 * - stdint.h
 *
 * @author LONG | R&D Dept. | EternalChip
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
#ifndef __EC_BSP_AHT21_DRIVER_H__ // Avoid repeated including same files later
#define __EC_BSP_AHT21_DRIVER_H__

//******************************** Defines **********************************//

#include <stdint.h> 
#include <stdio.h>

//******************************** Includes *********************************//

//******************************** Defines **********************************//
#define INITED 1               /*LED is inited                               */
#define NOT_INITED 0           /*LED is not inited                           */

#define OS_SUPPORTING          /*OS_SUPPORTING depending on OS avaliable     */
#define DEBUG                  /*Enable DEBUG                                */
#define DEBUG_OUT(X) printf(X) /* DEBUG output innfoto indicate statu        */ 

typedef enum
{
    LED_OK = 0,             /* Operation completed successfully              */
    LED_ERROR = 1,          /* General error occurred                        */
    LED_ERRORTIMEOUT = 2,   /* Operation timed out                           */
    LED_ERRORSOURCE = 3,    /* Invalid source or resource error              */
    LED_ERRORPARAMETER = 4, /* Invalid parameter provided                    */
    LED_ERRORNOMEMORY = 5,  /* Memory allocation failed or insufficient      */
    LED_ERRORISR = 6,       /* Interrupt Service Routine (ISR) error         */
    LED_RESERVED = 0xff,    /* Reserved value for future use (255 in decimal)*/
} led_status_t;

typedef enum
{
    PROPORTION_1_3 = 0,    /* Ratio of 1:3                                   */
    PROPORTION_1_2 = 1,    /* Ratio of 1:2                                   */
    PROPORTION_1_1 = 2,    /* Ratio of 1:1 (Equal proportion)                */
    PROPORTION_x_x = 0xff, /* Custom ratio                                   */
} PROPORTION_t;

typedef struct
{
    led_status_t (*pf_led_on)(void);  /* Function pointer for turning LED on */
    led_status_t (*pf_led_off)(void); /* Function pointer for turning LED off*/
} led_operations_t;

typedef struct
{
    led_status_t (*pf_get_time_ms)(uint32_t *const);/* getting current time  */
} time_base_ms_t;

typedef struct
{
    led_status_t (*pf_os_delay_ms)(const uint32_t);/* getting current time   */
} os_delay_t;

typedef led_status_t (*pf_led_control_t)(
                            uint32_t,      /* cycle_time[ms]                 */
                            uint32_t,      /* blink_times[times]             */
                            PROPORTION_t   /* portportion_on_off             */
                                        );
typedef struct
{
    /**** Target of Internal Status *****/
    uint8_t                     is_inited;

    /******* target of features *********/                 
    /* The whole time of blink          */
    uint32_t                cycle_time_ms;
    /* The times of blink               */
    uint32_t                  blink_times;
    /* The time of light on and off     */
    PROPORTION_t        porportion_on_off;

    /******* target of IOs needed *******/
    /* The intersurface from core layer */
    led_operations_t      *p_led_ops_inst;
    time_base_ms_t        *p_time_base_ms;
    /*  The intersurface from os layer  */
#ifdef OS_SUPPORTING
    os_delay_t           *p_os_time_delay;
#endif // OS_SUPPORTING
    /********* target of APIs ***********/
    pf_led_control_t     pf_led_controler;

}bsp_led_driver_t;

//******************************** Defines **********************************//

//******************************** Declaring ********************************//
/**
 * @brief Instantiate the target of bsp_led_driver_t.
 *
 * Steps:
 * 1. Adding the Core interfaces into target of bsp_led_driver instance.
 * 2. Adding the OS interfaces into target of bsp_led_driver instance.
 * 3. Adding the timebase interface into target of bsp_led_driver instance.
 *
 * @param[in] self Pointer to the LED driver instance to be initialized
 * @param[in] led_ops Pointer to LED operations structure containing function
 *                    pointers
 * @param[in] time_base Pointer to time base configuration (OS mode only)
 * @param[in] os_delay Pointer to OS delay function implementation
 *
 * @return led_status_t : LED_STATUS_OK if initialization successful
 *                        LED_STATUS_INVALID_PARAM if any input pointer is NULL
 *                        LED_STATUS_ERROR if initialization fails
 */
led_status_t led_driver_t_inst(
    bsp_led_driver_t *const self,
    const led_operations_t *const led_ops,
#ifdef OS_SUPPORTING
    const os_delay_t *const os_delay,
#endif // OS_SUPPORTING
    const time_base_ms_t *const time_base);

//******************************** Declaring ********************************//
#endif // end of __EC_BSP_AHT21_DRIVER_H__
