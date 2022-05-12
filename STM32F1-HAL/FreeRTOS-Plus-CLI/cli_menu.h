
#ifndef __CLI_MENU_H__
#define __CLI_MENU_H__

#include <stdio.h>
#include "log.h"
#include "misc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLI_LOG     printf
#define CLI_LOG_D   log_info
#define CLI_LOG_I   log_info
#define CLI_LOG_E   log_err

#if !(defined(ARRAY_SIZE))
#define ARRAY_SIZE(arr)   (sizeof(arr) / sizeof((arr)[0]))
#endif 

#define TAB_SWAP_SPACE  4   /* TAB转换为空格数 */
#define CLI_MENU_DEP_MAX    8   /* CLI 菜单的最大深度 */

typedef struct _cli_menu_t {
    const char *name;   /* cli 菜单选项名称 */
    const char *help_info;  /* 菜单选项帮助信息 */
    int (*cli_callback)(const char *pcCmdStr, uint8_t ucParamOffset);   /* 菜单回调函数 */
    struct _cli_menu_t *sub_menu;   /* 子菜单 */
}cli_menu_t;

/***********************************************************************
 * @function   : cli_menu_echo_help
 * @description: 打印cli命令菜单
 * @param       *ptCliMenu, 菜单结构节点
 * @param       dep, 菜单深度
 * @return      {*}
***********************************************************************/
void cli_menu_echo_help(cli_menu_t *ptCliMenu, uint8_t dep);

/***********************************************************************
 * @function   : cli_menu_callback
 * @description: cli菜单选项回调函数
 * @param       *pcCmdStr, 菜单选项字符命令
 * @param       *ptSrcMenu, 菜单结构节点
 * @return      0:success
***********************************************************************/
int cli_menu_callback(const char *pcCmdStr, cli_menu_t *ptSrcMenu);


#ifdef __cplusplus
}
#endif
#endif
