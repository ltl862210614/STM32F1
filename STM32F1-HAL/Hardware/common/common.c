#include <stdio.h>

#include "button.h"
#include "led.h"

void peripheral_init(void)
{
    button_init();
    led_init();
}
