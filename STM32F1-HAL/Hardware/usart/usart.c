#include <stdio.h>
#include <string.h>

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

uint16_t g_uart1_rx_len = 0;
uint8_t g_uart1_rx_flag = 0;    //接收完成标志
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
    #if 1  
	while((PRINTF_USART->SR&0X40)==0);//循环发送,直到发送完毕   
    PRINTF_USART->DR = (uint8_t) ch;  
    #else   
    HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)&ch, 1, 0xffff); 
    #endif
	return ch;
}

/**
  * 函数功能: 重定向c库函数 getchar,scanf 到 DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  while(HAL_UART_Receive(&g_uart1_handle,&ch, 1, 0xffff)!=HAL_OK);

  return ch;
}
#endif

void usart1_init(void)
{
    #if USART1_EN
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init;
    /* usart tx */
    gpio_init.Pin = GPIO_PIN_9;
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    /* usart rx */
    gpio_init.Pin = GPIO_PIN_10;
    gpio_init.Mode = GPIO_MODE_AF_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
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
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(&g_uart1_handle);
    //HAL_UART_Receive_IT(&g_uart1_handle, g_uart1_rx_buf, UART1_BUF_LEN_MAX);
    #endif

    HAL_NVIC_EnableIRQ(USART1_IRQn);    //使能USART1中断通道
    HAL_NVIC_SetPriority(USART1_IRQn,3,3);  //抢占优先级3，子优先级3

    __HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_RXNE);//接收中断
	__HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_IDLE);//空闲中断
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
    #if 1
    static uint16_t offset = 0;
    uint8_t data = 0;
    /* 接收中断 */
    if (__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_RXNE) != RESET)
    {
        HAL_UART_Receive(&g_uart1_handle,&data,1,1000);
		//将数据放入缓冲区
        g_uart1_rx_buf[offset++] = data;
        if (offset >= UART1_BUF_LEN_MAX)
        {
            offset = 0;
        }
        __HAL_UART_CLEAR_FLAG(&g_uart1_handle,UART_FLAG_RXNE);
    }

    /* 空闲中断 */
    if (__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_IDLE) != RESET)
    {
        //一帧数据接收完成
        //while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);
        //printf("[%s:%d]buf:%s,%d\n", __func__, __LINE__, g_uart1_rx_buf, offset);

        g_uart1_rx_len = offset;
        g_uart1_rx_flag = 1;

        g_uart1_rx_buf[offset] = '\0';
        offset = 0;

        __HAL_UART_CLEAR_IDLEFLAG(&g_uart1_handle);
    }
    #else
    HAL_UART_IRQHandler(&g_uart1_handle);
    printf("[%s:%d]%d\n", __func__, __LINE__, HAL_UART_GetState(&g_uart1_handle));

    while (HAL_UART_GetState(&g_uart1_handle) != HAL_UART_STATE_READY)
    {
        ;
    }
    printf("[%s:%d]\n", __func__, __LINE__);

    while(HAL_UART_Receive_IT(&g_uart1_handle, g_uart1_rx_buf, UART1_BUF_LEN_MAX) != HAL_OK)
    {
        ;
    }

    printf("[%s:%d]buf:%s\n", __func__, __LINE__, g_uart1_rx_buf);
    #endif
}

uint16_t get_uart1_rx_buf(uint8_t buf[])
{
    if (g_uart1_rx_len)
    {
        memcpy(buf, g_uart1_rx_buf, g_uart1_rx_len);
    }

    return g_uart1_rx_len;
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
