#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f1xx_hal.h"
#include "sys.h"

#include "common.h"
#include "task_misc.h"

static void start_task(void)
{
	task_misc_init();
}

int main(void)
{
	HAL_Init();

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();
	
  peripheral_init();
  
  start_task();

  while (0)
  {
    ;
  }

  vTaskStartScheduler();

	return 0;
}

