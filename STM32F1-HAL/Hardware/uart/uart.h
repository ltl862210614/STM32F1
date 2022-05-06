#ifndef __UART_H__
#define __UART_H__

#include "app_conf.h"

#if UART1_EN
#define UART1_BUF_LEN_MAX 64
extern uint8_t g_uart1_rx_buf[UART1_BUF_LEN_MAX];
extern uint16_t g_uart1_rx_len;
extern uint8_t g_uart1_rx_flag;    //接收完成标志
#endif

void uart1_init(void);

#endif
