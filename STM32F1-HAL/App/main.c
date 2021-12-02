#include "stm32f1xx_hal.h"

void SystemClock_Config(void)
{
}

int main(void)
{
	HAL_Init();

  /* Configure the system clock to 64 MHz */
  SystemClock_Config();
	
	return 0;
}

