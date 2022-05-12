
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"

#include "FreeRTOS_CLI.h"
#include "cli_menu.h"

void print_space(uint32_t n)
{
    uint32_t cnt = n;
    while(cnt--) 
    {
        CLI_LOG(" ");
    }
}

static void print_menu_item(cli_menu_t *ptCliMenu, uint8_t dep)
{
    if (ptCliMenu == NULL)
    {
        return ;
    }
    if (ptCliMenu->name != NULL)
    {
        print_space(dep*TAB_SWAP_SPACE);
        CLI_LOG("%s", ptCliMenu->name);
    }
    else
    {
        return;
    }

    if (ptCliMenu->help_info != NULL)
    {
        CLI_LOG(" %s", ptCliMenu->help_info);
    }
    CLI_LOG("\r\n");
}

/***********************************************************************
 * @function   : 
 * @description: 
 * @param       *ptCliMenu
 * @param       dep
 * @return      {*}
***********************************************************************/
void cli_menu_echo_help(cli_menu_t *ptCliMenu, uint8_t dep)
{
    cli_menu_t *pMenu = ptCliMenu;

    while(pMenu != NULL)
    {
        if (pMenu->name == NULL)
        {
            break;
        }
        
        //CLI_LOG_D("[%s:%d]menu=%p, name=%s, dep=%d\r\n", __func__, __LINE__, pMenu, pMenu->name, dep);
        print_menu_item(pMenu, dep);

        if(pMenu->sub_menu != NULL)
        {
            if (dep >= CLI_MENU_DEP_MAX)
            {
                CLI_LOG_E("echo depth is overflow\r\n");
                continue;
            }
            cli_menu_echo_help(pMenu->sub_menu, dep+1);
        }

        pMenu++;
    }
}

int cli_menu_callback(const char *pcCmdStr, cli_menu_t *ptSrcMenu)
{
    BaseType_t lParamrLen = 0;
	UBaseType_t ulParaNo = 1U;
    uint8_t dep = 0;
	int iRet = pdTRUE;

    //CLI_LOG_D("[%s:%d]pcCmdStr=%s>>\r\n", __func__, __LINE__, pcCmdStr);
    const char *pcParam = ( char * ) FreeRTOS_CLIGetParameter( pcCmdStr, ulParaNo++, &lParamrLen );
    if ( !pcParam )
    {
        return iRet;
    }

    cli_menu_t *pMenu = ptSrcMenu;
    while (NULL != pMenu)
    {
        if (pMenu->name == NULL || NULL == pcParam)
        {
            break;
        }
        
        if (strncmp(pMenu->name, pcParam, lParamrLen) != 0)
        {
            pMenu++;
            continue;
        }

        if(pMenu->cli_callback != NULL) 
        {
            /* cmd format is right, and execute the command callback function*/
            iRet = pMenu->cli_callback(pcParam, ulParaNo);
            break;
        }
        else 
        {
            if(pMenu->sub_menu != NULL)
            {
                dep++;
                if (dep >= CLI_MENU_DEP_MAX)
                {
                    CLI_LOG_E("menu depth is overflow\r\n");
                    return iRet;
                }
                pMenu = pMenu->sub_menu;
                pcParam = ( char * ) FreeRTOS_CLIGetParameter( pcCmdStr, ulParaNo++, &lParamrLen );
                if ( !pcParam )
                {
                    return iRet;
                }
            }
        }
    }

    if (pMenu == NULL)
    {
        CLI_LOG_E("cli error,use '<cmd> help' for more details.\r\n");
    }

    return pdFALSE;
}
