#include <stdio.h>
#include <string.h>

#include "flexible_button.h"
#include "button.h"

static flex_button_t gt_flex_button[BUTTON_ID_MAX];

static uint8_t flex_button_read(void *arg)
{
    uint8_t value = 0;

    flex_button_t *btn = (flex_button_t *)arg;

    value = read_button_pin(btn->id);

    return value;
}

static void flex_button_event_callback(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    if (btn->id >= BUTTON_ID_MAX)
    {
        return;
    }
	
    if ((flex_button_event_read(&gt_flex_button[btn->id]) == FLEX_BTN_PRESS_CLICK))
    {
        //printf("[combination]: button 0\n");
    }
}

void flex_button_event_handler(void)
{
    flex_button_scan();
}

void flex_btn_init(void)
{
    uint8_t i;

    memset(&gt_flex_button[0], 0x0, sizeof(gt_flex_button));
    for (i = 0; i < BUTTON_ID_MAX; i ++)
    {
        gt_flex_button[i].id = i;
        gt_flex_button[i].usr_button_read = flex_button_read;
        gt_flex_button[i].cb = flex_button_event_callback;
        gt_flex_button[i].pressed_logic_level = 0;
        gt_flex_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
        gt_flex_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(3000);
        gt_flex_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);

        if (i == BUTTON_ID_K2)
        {
            gt_flex_button[i].pressed_logic_level = 1;
        }

        flex_button_register(&gt_flex_button[i]);
    }
}
