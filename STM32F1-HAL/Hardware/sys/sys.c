#include "stm32f1xx_hal.h"

void SystemClock_Config(void)
{
    HAL_StatusTypeDef iRet = HAL_OK;
    RCC_ClkInitTypeDef rccClkDef = 
    {
        .ClockType = (RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2),
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV2,
        .APB2CLKDivider = RCC_HCLK_DIV1
    };

    RCC_OscInitTypeDef rccOscDef = 
    {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_HSE_ON,
        .HSEPredivValue = RCC_HSE_PREDIV_DIV1,
        //.LSEState = RCC_LSE_OFF,
        //.HSIState = RCC_HSI_OFF,
        //.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT,
        //.LSIState = RCC_LSI_OFF,
        .PLL = 
        {
            .PLLState = RCC_PLL_ON,
            .PLLSource = RCC_PLLSOURCE_HSE,
            .PLLMUL = RCC_PLL_MUL9
        }
    };

    /* 先配置Osc,再配置Clock,否则时钟异常系统无法正常运行 */
    iRet = HAL_RCC_OscConfig(&rccOscDef);
    if (iRet != HAL_OK)
    {
        while (1);
    }

    iRet = HAL_RCC_ClockConfig(&rccClkDef, FLASH_LATENCY_2);
    if (iRet != HAL_OK)
    {
        while (1);
    }
}
