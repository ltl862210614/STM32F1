#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_cortex.h"

#include "usart.h"

#define PRINTF_USART USART2

#if USART1_EN
#define USART1_BUF_LEN_MAX 64
uint8_t g_usart1_rx_buf[USART1_BUF_LEN_MAX];
static USART_HandleTypeDef g_usart1_handle;
#endif

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((PRINTF_USART->SR&0X40)==0);//循环发送,直到发送完毕   
    PRINTF_USART->DR = (uint8_t) ch;      
	return ch;
}

void usart1_init(void)
{
    #if USART1_EN
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init;
    /* usart tx */
    gpio_init.Pin = GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    /* usart rx */
    gpio_init.Pin = GPIO_PIN_10;
    gpio_init.Mode = GPIO_MODE_AF_INPUT;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    __HAL_RCC_USART1_CLK_ENABLE();
    
    g_usart1_handle.Instance = USART1;
    g_usart1_handle.Init.BaudRate = 9600;
    g_usart1_handle.Init.WordLength = USART_WORDLENGTH_8B;
    g_usart1_handle.Init.StopBits = USART_STOPBITS_1;
    g_usart1_handle.Init.Parity = USART_PARITY_NONE;
    g_usart1_handle.Init.Mode = USART_MODE_TX_RX;
    HAL_USART_Init(&g_usart1_handle);
    HAL_USART_Receive_IT(&g_usart1_handle, g_usart1_rx_buf, USART1_BUF_LEN_MAX);

    HAL_NVIC_EnableIRQ(USART1_IRQn);    //使能USART1中断通道
    HAL_NVIC_SetPriority(USART1_IRQn,3,3);  //抢占优先级3，子优先级3
    #endif
}

/***********************************************************************
 * @function   : USART1_IRQHandler
 * @description: USART1 interrupt service function
 * @param       {*}
 * @return      {*}
***********************************************************************/
void USART1_IRQHandler(void)                	
{ 
}

/***********************************************************************
 * @function   : HAL_UART_RxCpltCallback
 * @description: usart Rx Transfer completed callbacks
 * @param       *huart
 * @return      {*}
***********************************************************************/
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
	if(husart->Instance==USART1)//如果是串口1
	{
    }
}
