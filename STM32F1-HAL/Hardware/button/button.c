#include <stdio.h>

//#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_gpio.h"

#include "misc.h"
#include "button.h"

static button_def_t gt_btn_def[BUTTON_ID_MAX] = 
{
    {BUTTON_ID_K2, GPIOA, {GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
    {BUTTON_ID_K3, GPIOA, {GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH}},
};

button_def_t *get_btn_def(uint8_t btn_id)
{
    uint8_t i;
    uint8_t flag = 0;
    uint8_t size = SIZE_ARR(gt_btn_def);
    if (btn_id >= size)
    {
        return NULL;
    }

    for (i=0; i<size; i++)
    {
        if (gt_btn_def[i].btn_id == btn_id)
        {
            flag = 1;
            break;
        }
    }

    if (flag)
    {
        return &gt_btn_def[i];
    }
    else
    {
        return NULL;
    }
}

void button_init(void)
{
    uint8_t i;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    uint8_t size = SIZE_ARR(gt_btn_def);

    #if 1
    for (i=0; i<size; i++)
    {
        button_def_t *pt_gpio = get_btn_def(i);
        if (pt_gpio != NULL)
        {
            HAL_GPIO_Init(pt_gpio->gpio, &pt_gpio->init);
        }
    }
    #else
    button_def_t *pt_gpio = get_btn_def(BUTTON_ID_K2);
    if (pt_gpio != NULL)
    {
        HAL_GPIO_Init(pt_gpio->gpio, &pt_gpio->init);
    }
    #endif
}

uint8_t read_button_pin(uint8_t btn_id)
{
    uint8_t ret = 0xff;

    button_def_t *pt_gpio = get_btn_def(btn_id);
    if (pt_gpio != NULL)
    {
        ret = HAL_GPIO_ReadPin(pt_gpio->gpio, pt_gpio->init.Pin);
    }

    return ret;
}
