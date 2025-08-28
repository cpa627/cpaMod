#ifndef __STM32_FLASH_H
#define __STM32_FLASH_H

/*
 * 对于stm32f103c8t6 
 * 它的flash是64kB，但是f10xxx的闪存手册中的小容量--32 * 1kB；中容量--128 * 1kB；大容量--256 * 2kB
*/

/*
 * 对于stm32f407zgt6
 * 它的flash是1024kB，f40x和f41x的flash构成--4*16kB + 1*64kB + 7*128kB
*/

/*
 * 对于stm32f411ceu6
 * 它的flash是512kB，4*16kB + 1*64kB + 3*128kB
*/

/*
 * 对于stm32G431rbt6，可选择是否启用双bank
 * 它的flash是128kB，双bank mode:32*2kB * 32*2kB     单bank mode:32*4kB
*/

#endif