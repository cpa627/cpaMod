#include "string_utils.h"


/**
 * @brief 
 * 
 * @param dest Ŀ���ַ
 * @param src  Դ��ַ
 * @param size �����ֽ���
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
