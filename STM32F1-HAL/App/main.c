#include "stm32f1xx_hal.h"
#include "sys.h"

int main(void)
{
	HAL_Init();

  /* Configure the system clock to 64 MHz */
  SystemClock_Config();
	
	return 0;
}

