#ifndef __FLASH_H__
#define __FLASH_H__

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_SIZE_K  (128)     // K 
#define FLASH_SIZE  (FLASH_SIZE_K * 1024)   // bytes

#if 0
#if FLASH_SIZE_K < 256
    #define FLASH_PAGE_SIZE   1024    // bytes
#else
    #define FLASH_PAGE_SIZE   2048
#endif
#endif

#define FLASH_BASE_ADDR 0x08000000
#define FLASH_ADDR_END (FLASH_BASE_ADDR + FLASH_SIZE)

void flash_init(void);
int flash_read(uint32_t addr, void *buf, uint16_t length);
int flash_write(uint32_t addr, void *buf, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
