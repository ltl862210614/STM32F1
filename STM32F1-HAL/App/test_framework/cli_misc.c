
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"
#include "cli_menu.h"

#define MISC_CLI_NAME "misc"

static portBASE_TYPE cli_misc_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

static int cli_misc_echo_help(const char *pcCmdStr, uint8_t ucParamOffset);

static cli_menu_t misc_cli_menu[] = {
    {"help", NULL, cli_misc_echo_help, NULL},
    { NULL }
};

static const CLI_Command_Definition_t gt_cli_misc =
{
	MISC_CLI_NAME, /* The command string to type. */
    MISC_CLI_NAME"\r\n  Usage:Use '"MISC_CLI_NAME" help' command for more details\r\n",
	cli_misc_command, /* The function to run. */
	-1 /* The user can enter any number of commands. */
};

static int cli_misc_echo_help(const char *pcCmdStr, uint8_t ucParamOffset)
{
    UNUSED(pcCmdStr);
    UNUSED(ucParamOffset);

    CLI_LOG("%s\r\n", MISC_CLI_NAME);
    cli_menu_echo_help(misc_cli_menu, 1);

    return pdFALSE;
}

static portBASE_TYPE cli_misc_command(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	int iRet = pdTRUE;

    pcWriteBuffer[0] = '\0';
    UNUSED(xWriteBufferLen);

    iRet = cli_menu_callback(pcCommandString, misc_cli_menu);
    if (iRet)
    {
        //snprintf(pcWriteBuffer, xWriteBufferLen, "cli error,Use 'help' to get more details\r\n");
    }

	return pdFALSE;
}

void vRegisterMiscCLICommands( void )
{
    FreeRTOS_CLIRegisterCommand( &gt_cli_misc );
}
