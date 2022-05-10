#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdint.h>

#include "uart.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    LOG_STA_OK,
    LOG_STA_NO_INIT,
    LOG_STA_LEV_OVER,
    LOG_STA_ERR
}log_status_e;

typedef enum{
    LOG_LEV_ERR = 1,
    LOG_LEV_WARN,
    LOG_LEV_INFO,
    LOG_LEV_DBG,

    LOG_LEV_ALL
}log_level_e;

typedef struct _log_param_t
{
    uint8_t init;   /* 0:success,else:failed */
    uint8_t lev;
}log_param_t;

extern log_param_t g_log_param;

int log_level(const uint8_t lev, const char  *fmt_s, ...);
void log_init(void);
int set_log_lev(uint8_t lev);

#if PRINTF_UART_EN
#define log_err(...)    printf(__VA_ARGS__)
#define log_warn(...)   printf(__VA_ARGS__)
#define log_info(...)   printf(__VA_ARGS__)
#define log_dbg(...)    printf(__VA_ARGS__)
#else
#define log_err(...) 	log_level(LOG_LEV_ERR, __VA_ARGS__);
#define log_warn(...) 	log_level(LOG_LEV_WARN, __VA_ARGS__);
#define log_info(...) 	log_level(LOG_LEV_INFO, __VA_ARGS__);
#define log_dbg(...) 	log_level(LOG_LEV_DBG, __VA_ARGS__);
#endif

#ifdef __cplusplus
}
#endif

#endif
