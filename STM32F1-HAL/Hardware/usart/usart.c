#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_cortex.h"

#include "usart.h"

#define PRINTF_USART USART1

#if USART1_EN
#if 0
#define USART1_BUF_LEN_MAX 64
uint8_t g_usart1_rx_buf[USART1_BUF_LEN_MAX];
static USART_HandleTypeDef g_usart1_handle;
#else
#define UART1_BUF_LEN_MAX 64
uint8_t g_uart1_rx_buf[UART1_BUF_LEN_MAX];
static UART_HandleTypeDef g_uart1_handle;
#endif
#endif

//使USART串口可用printf函数发送
//在usart.h文件里可更换使用printf函数的串口号	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE {
	int handle; 
}; 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x){ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((PRINTF_USART->SR&0X40)==0);//循环发送,直到发送完毕   
    PRINTF_USART->DR = (uint8_t) ch;      
	return ch;
}
#endif

void usart1_init(void)
{
    #if USART1_EN
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init;
    /* usart tx */
    gpio_init.Pin = GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_AF_PP;
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

    #if 0
    g_usart1_handle.Instance = USART1;
    g_usart1_handle.Init.BaudRate = 115200;
    g_usart1_handle.Init.WordLength = USART_WORDLENGTH_8B;
    g_usart1_handle.Init.StopBits = USART_STOPBITS_1;
    g_usart1_handle.Init.Parity = USART_PARITY_NONE;
    g_usart1_handle.Init.Mode = USART_MODE_TX_RX;
    HAL_USART_Init(&g_usart1_handle);
    HAL_USART_Receive_IT(&g_usart1_handle, g_usart1_rx_buf, USART1_BUF_LEN_MAX);
    #else
    g_uart1_handle.Instance = USART1;
    g_uart1_handle.Init.BaudRate = 115200;
    g_uart1_handle.Init.WordLength = USART_WORDLENGTH_8B;
    g_uart1_handle.Init.StopBits = USART_STOPBITS_1;
    g_uart1_handle.Init.Parity = USART_PARITY_NONE;
    g_uart1_handle.Init.Mode = USART_MODE_TX_RX;
    HAL_UART_Init(&g_uart1_handle);
    HAL_UART_Receive_IT(&g_uart1_handle, g_uart1_rx_buf, UART1_BUF_LEN_MAX);
    #endif

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
