#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_cortex.h"

#include "uart.h"

#define PRINTF_UART USART1
#define PRINTF_HANDLE g_uart1_handle

#if UART1_EN
volatile uint8_t g_uart1_rx_buf[UART1_BUF_LEN_MAX];
static UART_HandleTypeDef g_uart1_handle;

uint16_t g_uart1_rx_len = 0;
volatile uint8_t g_uart1_rx_flag = 0;    //接收完成标志

#if UART1_DMA_EN
static DMA_HandleTypeDef g_hdma_uart1_tx;
static DMA_HandleTypeDef g_hdma_uart1_rx;
#endif

#endif

int is_in_isr (void) //若在中断中__get_IPSR()返回1，否则返回0
{
   return __get_IPSR();  
}

#define PRINTF_LOCK_EN 0
static char printf_buf[128] = {0};
int uart_print(char *format, ...)
{
    int ret;
    //char printf_buf[128] = {0};
 
    #if PRINTF_LOCK_EN
    if(is_in_isr() != 0)
	{
        taskDISABLE_INTERRUPTS();//若在中断中调用则关闭中断，防止中断嵌套造成线程不安全
	}
    else
    {
        #if 0
		taskENTER_CRITICAL();    //若不在中断中则进入临界区关闭中断且禁止任务调度,消耗资源
        #else
        while(HAL_UART_GetState(&PRINTF_HANDLE) == HAL_UART_STATE_BUSY_TX)//若串口忙则挂起此任务
		taskYIELD();
        #endif
	}
    #endif

	va_list ap;
	va_start(ap, format);
	vsprintf(printf_buf, format, ap);

	ret = HAL_UART_Transmit(&PRINTF_HANDLE, (uint8_t *)printf_buf, strlen(printf_buf), 0xffff);
	va_end(ap);

    #if PRINTF_LOCK_EN
	if(is_in_isr() != 0)
	{
		taskENABLE_INTERRUPTS();//打开中断
	}
    #if 0
    else
    {
		taskEXIT_CRITICAL();//退出临界区
	}
    #endif
    #endif

    return ret;
}

//使UART串口可用printf函数发送
//在uart.h文件里可更换使用printf函数的串口号	  
#if PRINTF_UART_EN
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
	while((PRINTF_UART->SR&0X40)==0);//循环发送,直到发送完毕   
    PRINTF_UART->DR = (uint8_t) ch;  
    #else 
    while(__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_TC) == HAL_OK);
    HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)&ch, 1, 0xffff); 
    #endif

	return ch;
}

/**
  * 函数功能: 重定向c库函数 getchar,scanf 到 DEBUG_UARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE * f)
{
    uint8_t ch = 0;
    while(HAL_UART_Receive(&g_uart1_handle,&ch, 1, 0xffff) != HAL_OK);

    return ch;
}
#endif

#if UART1_EN
static void uart1_init(void)
{
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

    g_uart1_handle.Instance = USART1;
    g_uart1_handle.Init.BaudRate = 115200;
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&g_uart1_handle);
    //HAL_UART_Receive_IT(&g_uart1_handle, g_uart1_rx_buf, UART1_BUF_LEN_MAX);

    HAL_NVIC_EnableIRQ(USART1_IRQn);    //使能USART1中断通道
    HAL_NVIC_SetPriority(USART1_IRQn,3,0);  //抢占优先级3，子优先级3

    __HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_RXNE);//接收中断
	__HAL_UART_ENABLE_IT(&g_uart1_handle,UART_IT_IDLE);//空闲中断

    #if UART1_DMA_EN
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    //Rx DMA 配置
    g_hdma_uart1_rx.Instance = DMA1_Channel5;
    g_hdma_uart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_hdma_uart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    g_hdma_uart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    g_hdma_uart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_hdma_uart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_hdma_uart1_rx.Init.Mode = DMA_NORMAL;
    g_hdma_uart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;

    HAL_DMA_DeInit(&g_hdma_uart1_rx);   
    HAL_DMA_Init(&g_hdma_uart1_rx);
    __HAL_LINKDMA(&g_uart1_handle,hdmarx,g_hdma_uart1_rx); 
    if(HAL_UART_Receive_DMA(&g_uart1_handle, (uint8_t *)g_uart1_rx_buf, UART1_BUF_LEN_MAX) != HAL_OK)
    {
    }

    //Tx DMA 配置
    g_hdma_uart1_tx.Instance = DMA1_Channel4;
    g_hdma_uart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    g_hdma_uart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    g_hdma_uart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    g_hdma_uart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    g_hdma_uart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    g_hdma_uart1_tx.Init.Mode = DMA_NORMAL;
    g_hdma_uart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_DeInit(&g_hdma_uart1_tx);   
    HAL_DMA_Init(&g_hdma_uart1_tx);
    __HAL_LINKDMA(&g_uart1_handle,hdmatx,g_hdma_uart1_tx); 
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
    //HAL_UART_IRQHandler(&g_uart1_handle); // 可省略
    #if UART1_DMA_EN
    /* 空闲中断 */
    if (__HAL_UART_GET_FLAG(&g_uart1_handle, UART_FLAG_IDLE) != RESET)
    {
        HAL_UART_DMAStop(&g_uart1_handle);
        g_uart1_rx_len  = UART1_BUF_LEN_MAX - __HAL_DMA_GET_COUNTER(&g_hdma_uart1_rx);
        //printf("[%s:%d]buf:%s,%d\n", __func__, __LINE__, g_uart1_rx_buf, g_uart1_rx_len);
        //HAL_UART_Transmit(&g_uart1_handle, g_uart1_rx_buf, g_uart1_rx_len, 0xffff);
        
        g_uart1_rx_flag = 1;

        //memset(g_uart1_rx_buf,0x00,g_uart1_rx_len);
        HAL_UART_Receive_DMA(&g_uart1_handle, (uint8_t*)g_uart1_rx_buf, UART1_BUF_LEN_MAX);
        __HAL_UART_CLEAR_IDLEFLAG(&g_uart1_handle);
    }
    #else
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
        while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC) != SET);
        printf("[%s:%d]buf:%s,%d\n", __func__, __LINE__, g_uart1_rx_buf, offset);

        g_uart1_rx_len = offset;
        g_uart1_rx_flag = 1;

        g_uart1_rx_buf[offset] = '\0';
        offset = 0;

        __HAL_UART_CLEAR_IDLEFLAG(&g_uart1_handle);
    }
    #endif
}

uint16_t get_uart1_rx_buf(uint8_t buf[])
{
    memcpy(buf, (void *)g_uart1_rx_buf, g_uart1_rx_len);

    return g_uart1_rx_len;
}
#endif

/***********************************************************************
 * @function   : HAL_UART_RxCpltCallback
 * @description: uart Rx Transfer completed callbacks
 * @param       *huart
 * @return      {*}
***********************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)//如果是串口1
	{
    }
}

void uart_init(void)
{
    #if UART1_EN
    uart1_init();
    #endif
}
