#include "stm32_flash.h"
#include "flash_cmd.h"

#include "string_utils.h"

/* STM32 F103 FLASH OPERATION */

void stm32_flash_init()
{

}

unsigned int stm32_flash_read_byte(unsigned int addr)
{
    return *(unsigned char *)(addr);
}

unsigned int stm32_flash_read(unsigned char *buf, unsigned int addr, unsigned int len)
{
    mymemcpy(buf, (unsigned char *)addr, len);
    return len;
}

unsigned int stm32_flash_write(unsigned char *buf, unsigned int addr, unsigned int len)
{
    unsigned int end_addr = addr + len;
    unsigned int start_addr = addr;

    

}











static struct flash_t stm32_flash = {
    .init = stm32_flash_init,
    // .erase = stm32_flash_erase,
    .read = stm32_flash_read,
    // .write = stm32_flash_write,
};

