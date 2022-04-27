#include <stdio.h>

#include "misc.h"
#include "led.h"

static led_def_t g_led_def[LED_ID_MAX] =
{
    {LED_ID_1, GPIOB, {GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
    {LED_ID_2, GPIOB, {GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
};

led_def_t *get_led_def(uint8_t led_id)
{
    uint8_t i;
    uint8_t flag = 0;
    uint8_t size = SIZE_ARR(g_led_def);
    if (led_id >= size)
    {
        return NULL;
    }

    for (i=0; i<size; i++)
    {
        if (g_led_def[i].led_id == led_id)
        {
            flag = 1;
            break;
        }
    }

    if (flag)
    {
        return &g_led_def[i];
    }
    else
    {
        return NULL;
    }
}

void led_init(void)
{
    uint8_t i;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    uint8_t size = SIZE_ARR(g_led_def);

    for (i=0; i<size; i++)
    {
        led_def_t *p_gpio = get_led_def(i);
        if (p_gpio != NULL)
        {
            HAL_GPIO_Init(p_gpio->gpio, &p_gpio->init);
        }
    }
}

/***********************************************************************
 * @function   : led_switch
 * @description: led 开关控制
 * @param       led_id: led id
 * @param       on_off: 1-on, 0-off
 * @return      {0:success, else:failed}
***********************************************************************/
uint8_t led_switch(led_id_e led_id, uint8_t on_off)
{
    led_def_t *p_gpio = get_led_def(led_id);
    if (p_gpio == NULL)
    {
        return 1;
    }

    HAL_GPIO_WritePin(p_gpio->gpio, p_gpio->init.Pin, (GPIO_PinState)on_off);

    return 0;
}
