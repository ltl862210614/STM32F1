#include <stdio.h>

#include "button.h"
#include "led.h"
#include "uart.h"
#include "common.h"

void peripheral_init(void)
{
    button_init();
    led_init();
    uart_init();
    log_init();
}
