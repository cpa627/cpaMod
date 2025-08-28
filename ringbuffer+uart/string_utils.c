#include "string_utils.h"


/**
 * @brief 
 * 
 * @param dest 目标地址
 * @param src  源地址
 * @param size 拷贝字节数
 */
void mymemcpy(void *dest, void *src, int size)
{
    char *dest_ptr = (char *)dest;
    char *src_ptr = (char *)src;

    while(size-- > 0)
    {
        *dest_ptr++ = *src_ptr++;
    }
}
