#ifndef __LED_H__
#define __LED_H__

#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

typedef enum
{
    LED_ID_1,
    LED_ID_2,
    LED_ID_MAX,
}led_id_e;

typedef struct _led_def_t
{
    led_id_e led_id;
    GPIO_TypeDef *gpio;
    GPIO_InitTypeDef init;
}led_def_t;

void led_init(void);

/***********************************************************************
 * @function   : led_switch
 * @description: led 开关控制
 * @param       led_id: led id
 * @param       on_off: 1-on, 0-off
 * @return      {0:success, else:failed}
***********************************************************************/
uint8_t led_switch(led_id_e led_id, uint8_t on_off);

#endif
