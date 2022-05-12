#ifndef __UART_H__
#define __UART_H__

#include "FreeRTOS.h"
#include "semphr.h"

#include "app_conf.h"

#define PRINTF_UART_EN 1

#if UART1_EN
#define UART1_BUF_LEN_MAX 64
extern volatile uint8_t g_uart1_rx_buf[UART1_BUF_LEN_MAX];
extern uint16_t g_uart1_rx_len;
extern volatile uint8_t g_uart1_rx_flag;    //接收完成标志

uint16_t get_uart1_rx_buf(uint8_t buf[]);
#endif

void uart_init(void);

int uart_print(char *format, ...);

#endif
