#ifndef APP_VERSION_H_
#define APP_VERSION_H_

#define VERSION_NAME                "STM32F1-HAL"
#define VERSION_MAJOR               0x1
#define VERSION_MINOR               0x0
#define VERSION_VER                 0x0


#define VERSION_U32                 ((VERSION_MAJOR << 16) | (VERSION_MINOR << 8) | (VERSION_VER))

#define VERSION_MAJORGET(x)         (((x) >> 16) & 0xFF)
#define VERSION_MINORGET(x)         (((x) >> 8) & 0xFF)
#define VERSION_VERGET(x)           (((x)) & 0xFF)


#endif

