/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Library includes. */
#include "stm32f1xx_hal.h"
#include "sys.h"

#include "common.h"
#include "task_misc.h"
#include "task_uart_cli.h"

static void start_task(void)
{
	task_misc_init();
  task_uart_cli_init();
}

int main(void)
{
	HAL_Init();

  /* Configure the system clock to 72 MHz */
  SystemClock_Config();
	
  peripheral_init();

  log_info("build on:\r\n\t%s %s\r\n", __DATE__, __TIME__);

  start_task();

  vTaskStartScheduler();

  while (0)
  {
    ;
  }

	return 0;
}

