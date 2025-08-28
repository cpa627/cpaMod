#include "stm32_flash.h"
#include "flash_cmd.h"


/* STM32 F103 FLASH OPERATION */

void stm32_flash_init()
{

}

unsigned int stm32_flash_read(unsigned int addr)
{
    return *(unsigned int *)(addr);
}












static struct flash_t stm32_flash = {
    .init = stm32_flash_init,
    // .erase = stm32_flash_erase,
    .read = stm32_flash_read,
    // .write = stm32_flash_write,
};

