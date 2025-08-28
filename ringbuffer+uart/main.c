#include "ringbuffer.h"
#include "string_utils.h"

#include <stdio.h>

// ringbuffer 实例
unsigned char rxbuffer[20] = {0};

// 接收缓存
unsigned char test_buffer[100] = {0};

struct ringbuffer ringbuffer_tsk = {0};

#define NORMAL_WRITE_TEST      0        // 一般非覆盖写入
#define FORCE_WRITE_TEST       1


int main(void)
{

    // 初始化
    ringbuffer_init(&ringbuffer_tsk, rxbuffer, sizeof(rxbuffer));

    #if NORMAL_WRITE_TEST
    // 非覆盖写入数据验证 -- 模拟串口接收数据，写入rxbuffer
    ringbuffer_write(&ringbuffer_tsk, "1234567890", 10);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);
    ringbuffer_write(&ringbuffer_tsk, "hello world", 11);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // 读取数据验证 -- 读到新test_buffer中
    ringbuffer_read(&ringbuffer_tsk, test_buffer, 20);
    printf("test_buffer: %s\n", test_buffer);

    // 读取rxbuffer数据验证 -- 目前还没有覆盖写入新数据
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // 追加写数据验证
    printf("append write bytes test:\r\n");
    ringbuffer_write(&ringbuffer_tsk, "aaaaabbbbbcccccddddd", 20);  // 前面读了20bytes,
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // 普通追加写字节就会失败
    printf("append write byte test:\r\n");
    ringbuffer_write_char(&ringbuffer_tsk, 'p');
    ringbuffer_write_char(&ringbuffer_tsk, '?');
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    ringbuffer_read(&ringbuffer_tsk, rxbuffer, 2);
    ringbuffer_write_char(&ringbuffer_tsk, 'p');
    ringbuffer_write_char(&ringbuffer_tsk, '?');
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    #endif

    #if FORCE_WRITE_TEST
    // 覆盖写入数据验证
    ringbuffer_write_force(&ringbuffer_tsk, "1234567890", 10);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);
    ringbuffer_write_force(&ringbuffer_tsk, "hello world", 11);
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // 读取数据验证
    ringbuffer_read(&ringbuffer_tsk, test_buffer, 20);
    printf("test_buffer: %s\n", test_buffer);

    // 读取剩余数据验证
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // 追加写数据验证
    printf("append write bytes test:\r\n");
    ringbuffer_write_force(&ringbuffer_tsk, "aaaaabbbbbcccccdddddeeeee", 25);  // 前面读了20bytes,
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    // 强制追加写字节
    printf("append write byte test:\r\n");
    ringbuffer_write_char_force(&ringbuffer_tsk, 'p');
    ringbuffer_write_char_force(&ringbuffer_tsk, '?');
    printf("rxbuffer: %s\n", ringbuffer_tsk.buffer);

    #endif
    

    while(1)
    {

    }

    return 0;
}
