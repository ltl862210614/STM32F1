#include <stdio.h>

#include "button.h"
#include "led.h"
#include "uart.h"

void peripheral_init(void)
{
    button_init();
    led_init();
    uart1_init();
}
