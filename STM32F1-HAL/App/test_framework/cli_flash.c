
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

#include "stm32f1xx_hal.h"

#include "cli_menu.h"
#include "flash.h"

#define TEST_FLASH_ADDR (FLASH_ADDR_END - FLASH_SIZE)

#define FLASH_CLI_NAME "flash"

static portBASE_TYPE cli_flash_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

static int cli_flash_echo_help(const char *pcCmdStr, uint8_t ucParamOffset);
static int cli_falsh_init(const char *pcCmdStr, uint8_t ucParamOffset);
static int cli_falsh_read(const char *pcCmdStr, uint8_t ucParamOffset);
static int cli_falsh_write(const char *pcCmdStr, uint8_t ucParamOffset);

static cli_menu_t flash_cli_menu[] = {
    {"help", NULL, cli_flash_echo_help, NULL},
    {"init", NULL, cli_falsh_init, NULL},
    {"rd", NULL, cli_falsh_read, NULL},
    {"wr", NULL, cli_falsh_write, NULL},
    { NULL }
};

static const CLI_Command_Definition_t gt_cli_flash =
{
	FLASH_CLI_NAME, /* The command string to type. */
    FLASH_CLI_NAME"\r\n  Usage:Use '"FLASH_CLI_NAME" help' command for more details\r\n",
	cli_flash_command, /* The function to run. */
	-1 /* The user can enter any number of commands. */
};

static int cli_flash_echo_help(const char *pcCmdStr, uint8_t ucParamOffset)
{
    UNUSED(pcCmdStr);
    UNUSED(ucParamOffset);

    CLI_LOG("%s\r\n", FLASH_CLI_NAME);
    cli_menu_echo_help(flash_cli_menu, 1);

    return pdFALSE;
}

static int cli_falsh_init(const char *pcCmdStr, uint8_t ucParamOffset)
{
    UNUSED(pcCmdStr);
    UNUSED(ucParamOffset);

    CLI_LOG_D("[%s:%d]\r\n", __func__, __LINE__);
    flash_init();

    return pdFALSE;
}

static int cli_falsh_read(const char *pcCmdStr, uint8_t ucParamOffset)
{
    UNUSED(pcCmdStr);
    UNUSED(ucParamOffset);
    uint8_t buf[32] = {0};
    flash_read(TEST_FLASH_ADDR, buf, 32);
    CLI_LOG_D("buf:%d,%s\r\n", buf[0], buf);

    return pdFALSE;
}

static int cli_falsh_write(const char *pcCmdStr, uint8_t ucParamOffset)
{
    int ret = pdFALSE;
    long paramLen = 0;
    const char *pBuf = FreeRTOS_CLIGetParameter(pcCmdStr, 1, &paramLen);
    if (pBuf)
    {
        CLI_LOG_D("pBuf:%s\r\n", pBuf);
        ret = flash_write(TEST_FLASH_ADDR, (void *)pBuf, strlen(pBuf));
    }
    //CLI_LOG_D("ret=%d\r\n", ret);

    return ret;
}

static portBASE_TYPE cli_flash_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	int iRet = pdTRUE;

    pcWriteBuffer[0] = '\0';
    UNUSED(xWriteBufferLen);

    iRet = cli_menu_callback(pcCommandString, flash_cli_menu);
    if (iRet)
    {
        //snprintf(pcWriteBuffer, xWriteBufferLen, "cli error,Use 'help' to get more details\r\n");
    }

	return pdFALSE;
}

void vRegisterFlashCLICommands( void )
{
    FreeRTOS_CLIRegisterCommand( &gt_cli_flash );
}
