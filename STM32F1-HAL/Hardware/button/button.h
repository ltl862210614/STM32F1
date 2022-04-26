#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"

typedef enum
{
    //BUTTON_ID_K1,
    BUTTON_ID_K2,
    BUTTON_ID_K3,
    BUTTON_ID_MAX
} button_id_e;

typedef struct _button_def_t
{
    button_id_e btn_id;
    GPIO_TypeDef *gpio;
    GPIO_InitTypeDef init;
}button_def_t;

button_def_t *get_btn_def(uint8_t btn_id);
void button_init(void);
uint8_t read_button_pin(uint8_t btn_id);

#endif
