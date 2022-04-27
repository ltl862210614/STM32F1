#include <stdio.h>

#include "button.h"
#include "led.h"
#include "usart.h"

void peripheral_init(void)
{
    button_init();
    led_init();
    usart1_init();
}
