#include <stdio.h>
#include <string.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_flash.h"
#include "flash.h"
#include "log.h"

static uint8_t gPageBuf[FLASH_PAGE_SIZE];

/// 初始化FLASH
void flash_init(void)
{
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    HAL_FLASH_Lock();
}

uint16_t flash_read_halfword(uint32_t addr)
{
    return *(volatile uint16_t*)addr;
}

int flash_read(uint32_t addr, void *buf, uint16_t length)
{
    int ret = 0;

    #define R_TYPE uint32_t
    uint8_t typeLen = sizeof(R_TYPE);
    const uint8_t *pSrc = (const uint8_t *)addr;
    uint8_t *pDst = (uint8_t *)buf;
    uint16_t leftLen = length;  // the length remained to read

    if ((!buf) || (addr < FLASH_BASE_ADDR) || (addr > FLASH_ADDR_END) || (!length))
    {
        log_err("[%s:%d]\r\n", __func__, __LINE__);
        return ret;
    }

    while (leftLen >= typeLen && (R_TYPE *)pSrc <= (R_TYPE *)(FLASH_ADDR_END-typeLen))
    {
        *(R_TYPE *)pDst = *(R_TYPE *)pSrc;
        pSrc += typeLen;
        pDst += typeLen;
        leftLen -= typeLen;
        //log_dbg("[%s:%d]\r\n", __func__, __LINE__);
    }

    while (leftLen)
    {
        *pDst++ = *pSrc++;
        leftLen--;
        //log_dbg("[%s:%d]\r\n", __func__, __LINE__);
    }

    ret = length - leftLen;

    log_dbg("[%s:%d]%d\r\n", __func__, __LINE__, ret);
    return ret;
}

int8_t flash_erase_page(uint32_t pageAddr)
{
    #define ERASE_CHECK 1
    int ret = 0;
    uint32_t pageError = 0;
    if ((pageAddr < FLASH_BASE_ADDR) || (pageAddr > FLASH_ADDR_END))
    {
        log_err("[%s:%d]\r\n", __func__, __LINE__);
        return 1;
    }
    
    uint32_t pageNum = (pageAddr - FLASH_BASE_ADDR) / FLASH_PAGE_SIZE;
    pageAddr = FLASH_BASE_ADDR + (pageNum * FLASH_PAGE_SIZE);
    #if ERASE_CHECK
    ret = flash_read(pageAddr, gPageBuf, FLASH_PAGE_SIZE);
    if (ret != FLASH_PAGE_SIZE)
    {
        log_err("[%s:%d]%d\r\n", __func__, __LINE__, ret);
        return 0;
    }
    uint16_t i = 0;
    for (i=0;i<FLASH_PAGE_SIZE;i++)
    {
        if (gPageBuf[i] != 0xff)
        {
            break;
        }
    }
    if (i >= FLASH_PAGE_SIZE)
    {
        log_info("[%s:%d]\r\n", __func__, __LINE__);
        return 0;
    }
    #endif

	FLASH_EraseInitTypeDef tEraseInit;
	tEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	tEraseInit.PageAddress = pageAddr;
	tEraseInit.Banks = FLASH_BANK_1;
	tEraseInit.NbPages = pageNum;
    ret = HAL_FLASHEx_Erase(&tEraseInit, &pageError);
	if( ret!= HAL_OK)
	{
        log_err("[%s:%d]%d\r\n", __func__, __LINE__, ret);
		return ret;
	}
	return 0;
}

int flash_write_halfword(uint32_t addr, void *buf, uint16_t length)
{
    int ret = 0;

    return ret;
}

int flash_write_nocheck(uint32_t addr, void *buf, uint16_t length)
{
    int ret = 0;
    uint32_t writeType = FLASH_TYPEPROGRAM_HALFWORD;
    #define W_TYPE uint16_t
    uint8_t typeLen = sizeof(W_TYPE);
    const uint8_t *pDst = (const uint8_t *)addr;
    uint8_t *pSrc = (uint8_t *)buf;
    uint16_t leftLen = length;

    if ((!buf) || (addr < FLASH_BASE_ADDR) || (addr > FLASH_ADDR_END) || (!length))
    {
        log_err("[%s:%d]\r\n", __func__, __LINE__);
        return ret;
    }

    while (leftLen)
    {
        if (addr > (FLASH_ADDR_END - typeLen))
        {
            break;
        }

        HAL_FLASH_Program(writeType, addr, *(W_TYPE *)buf);
        /* write check */
        if (*(volatile W_TYPE *)addr != *(W_TYPE *)buf)
        {
            log_warn("[%s:%d]\r\n", __func__, __LINE__);
            break;
        }

        if(leftLen < typeLen)
        {
            leftLen = 0;
            break;
        }
        leftLen -= typeLen;
        pSrc += typeLen;
        pDst += typeLen;
    }

    ret = (length - leftLen);

    return ret;
}

int flash_write(uint32_t addr, void *buf, uint16_t length)
{
    int ret = 0;
    //uint16_t pageIdx = 0;
    uint16_t pageOffset = 0;    // addr 在页中的偏移
    uint16_t leftLen = length;  // 剩余未写的数据长度
    uint32_t addrOffset = addr; // addr 在 flash 中的偏移
    uint16_t size = 0;  // 每次从 buf 写入的数据长度
    uint8_t *pSrc = buf;

    if ((!buf) || (addr < FLASH_BASE_ADDR) || (addr > FLASH_ADDR_END) || (!length))
    {
        log_err("[%s:%d]\r\n", __func__, __LINE__);
        return ret;
    }

    //pageIdx = (addr - FLASH_BASE_ADDR) / FLASH_PAGE_SIZE;
    pageOffset = (addr - FLASH_BASE_ADDR) % FLASH_PAGE_SIZE;

    addrOffset -= pageOffset;
    /* 解锁FLASH */
    HAL_FLASH_Unlock();

    while (leftLen)
    {
        ret = flash_read(addrOffset, gPageBuf, FLASH_PAGE_SIZE);
        if (ret != FLASH_PAGE_SIZE)
        {
            log_err("[%s:%d]\r\n", __func__, __LINE__);
            return 0;
        }

        if (flash_erase_page(addrOffset) != 0)
        {
            log_err("[%s:%d]\r\n", __func__, __LINE__);
            break;
        }

        if (leftLen == length)
        {
            size = (leftLen <= (FLASH_PAGE_SIZE - pageOffset)) ? leftLen:(FLASH_PAGE_SIZE - pageOffset);
        }
        else
        {
            size = (leftLen < FLASH_PAGE_SIZE) ? leftLen:FLASH_PAGE_SIZE;
        }
        
        memcpy(&gPageBuf[pageOffset], (uint8_t *)pSrc, size);
        if (leftLen == length)
        {
            pageOffset = 0;
        }

        ret = flash_write_nocheck(addrOffset,gPageBuf, FLASH_PAGE_SIZE);
        if (ret != FLASH_PAGE_SIZE)
        {
            log_err("[%s:%d]\r\n", __func__, __LINE__);
            break;
        }
        addrOffset += FLASH_PAGE_SIZE;
        leftLen -= size;
        pSrc += size;
    }
    /* 加锁FLASH */
    HAL_FLASH_Lock();

    ret = length - leftLen;

    if (leftLen)
    {
        log_err("[%s:%d]failed[%d]\r\n", __func__, __LINE__, ret);
    }

    return ret;
}
