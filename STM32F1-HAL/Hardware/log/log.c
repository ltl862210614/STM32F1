#include <stdio.h>
#include <stdarg.h>

#include "log.h"

log_param_t g_log_param = {
    .init = 1,
    .lev = LOG_LEV_ALL
};

int log_level(const uint8_t lev, const char  *fmt_s, ...)
{
    int ret = LOG_STA_OK;

    if (g_log_param.init)
    {
        return LOG_STA_NO_INIT;
    }

    if (g_log_param.lev < lev)
    {
        return LOG_STA_LEV_OVER;
    }

    const char* prefix[LOG_LEV_ALL] = {"[ERR]", "[WARN]", "[INFO]", "[DBG]"};
    if (lev < LOG_LEV_ALL)
    {
        uart_print("%s", prefix[lev-1]);
    }

    va_list  ap;
    char buf[128] = {0};

    va_start(ap, fmt_s);
    vsprintf(buf, fmt_s, ap);
    ret = uart_print("%s", buf);
    va_end(ap);

    return ret;
}

/***********************************************************************
 * @function   : log_init
 * @description: log初始化,确保相应串口已初始化
 * @param       {*}
 * @return      {*}
***********************************************************************/
void log_init(void)
{
    g_log_param.init = 0;
    g_log_param.lev = LOG_LEV_ALL;
}

int set_log_lev(uint8_t lev)
{
    g_log_param.lev = lev;

    return 0;
}
