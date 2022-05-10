#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"

#include "task_manage.h"
#include "misc.h"

#include "uart.h"
#include "task_uart_cli.h"

char cOutputBuffer[ configCOMMAND_INT_MAX_OUTPUT_SIZE ] = { 0 };

static void task_uart_cli(void* param);

taskDef_t gtTaskUartCli = {
    .task_id = TASK_ID_UART_CLI,
    .task_name = "taskUartCli",
    .stack_depth = 128,
    .task_handle = NULL,
    .task_priority = 1,
    .task_func = task_uart_cli,
};

void task_uart_cli_init(void)
{
    task_create(&gtTaskUartCli);
}

void task_uart_cli_deInit(void)
{
	vTaskDelete(gtTaskUartCli.task_handle);
}

static void task_uart_cli(void* param)
{
    int ret = 0;
    uint8_t out_buf[32];
    uint8_t in_buf[UART1_BUF_LEN_MAX];
    log_dbg("%s start>>\r\n", __func__);
	UNUSED(param);

    while (1)
    {
        if (g_uart1_rx_flag)
        {
            memset(in_buf, 0, SIZE_ARR(in_buf));
            if (get_uart1_rx_buf(in_buf) == 0)
            {
                break;
            }
            do{
                log_dbg("[%s:%d]\r\n", __func__, __LINE__);
                ret = FreeRTOS_CLIProcessCommand((char *)in_buf, (char *)out_buf, SIZE_ARR(out_buf));
                if (ret == pdFALSE)
                {
                    log_dbg("%s\r\n", out_buf);
                    memset(out_buf, 0, SIZE_ARR(out_buf));
                }

                g_uart1_rx_flag = 0;
            }while(ret);
        }
        //log_dbg("[%s:%d]\r\n", __func__, __LINE__);
        //vTaskDelay(2000);
    }
    
}
