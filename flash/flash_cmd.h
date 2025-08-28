#ifndef __FLASH_CMD_H
#define __FLASH_CMD_H



struct flash_t
{
	unsigned char (*init)(void);
	unsigned int (*read)(unsigned char *buf, unsigned int addr, unsigned int len);
    unsigned int (*write)(unsigned char *buf, unsigned int addr, unsigned int len);
    unsigned int (*erase)(unsigned int addr, unsigned int len);
};






#endif
