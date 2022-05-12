
/* Standard includes. */
#include <stdint.h>
//#include <stdio.h>
//#include <string.h>

extern void vRegisterFlashCLICommands( void );
extern void vRegisterMiscCLICommands( void );

void tc_register_cli_commands(void)
{
    vRegisterFlashCLICommands();
    vRegisterMiscCLICommands();
}
